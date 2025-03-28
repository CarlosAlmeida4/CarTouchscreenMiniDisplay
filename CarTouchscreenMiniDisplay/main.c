#include <stdio.h>
#include "pico/stdlib.h"
#include "DEV_Config.h"
#include "LCD_1in28.h"
#include "CST816S.h"
#include "QMI8658.h"
//#include "lvgl.h"
#include "src/core/lv_obj.h"
#include "src/misc/lv_area.h"
#include "ui.h"


#define DISP_HOR_RES 240
#define DISP_VER_RES 240

#define INPUTDEV_TS  1

void LVGL_Init(void);
void Widgets_Init(void);

// LVGL
static lv_display_t *disp;
//static lv_disp_draw_buf_t disp_buf; //FIXME: Check for deletion
static lv_color_t buf0[DISP_HOR_RES * DISP_VER_RES/2];
static lv_color_t buf1[DISP_HOR_RES * DISP_VER_RES/2];
//static lv_disp_drv_t disp_drv;

//static lv_indev_drv_t indev_ts;//FIXME: Check for deletion
//static lv_indev_drv_t indev_en;//FIXME: Check for deletion
static lv_group_t *group;

static lv_obj_t *label_imu;

// Input Device 
static int16_t encoder_diff;
static lv_indev_state_t encoder_act;

static uint16_t ts_x;
static uint16_t ts_y;
static lv_indev_state_t ts_act;

// Timer 
static struct repeating_timer lvgl_timer;
static struct repeating_timer imu_data_update_timer;
static struct repeating_timer imu_diff_timer;

static void disp_flush_cb(lv_display_t * disp, const lv_area_t * area, uint8_t * color_p);
static void touch_callback(uint gpio, uint32_t events);
static void ts_read_cb(lv_indev_t * drv, lv_indev_data_t*data);
static void get_diff_data(void);
static void encoder_read_cb(lv_indev_t * drv, lv_indev_data_t*data);
static void dma_handler(void);
static bool repeating_lvgl_timer_callback(struct repeating_timer *t); 
static bool repeating_imu_data_update_timer_callback(struct repeating_timer *t); 
static bool repeating_imu_diff_timer_callback(struct repeating_timer *t);

/********************************************************************************
function:	Initializes LVGL and enbable timers IRQ and DMA IRQ
parameter:
********************************************************************************/
void LVGL_Init(void)
{
    // /*1.Init Timer*/ 
    add_repeating_timer_ms(500, repeating_imu_data_update_timer_callback, NULL, &imu_data_update_timer);
    add_repeating_timer_ms(50, repeating_imu_diff_timer_callback,        NULL, &imu_diff_timer);
    add_repeating_timer_ms(5,   repeating_lvgl_timer_callback,            NULL, &lvgl_timer);
    
    // /*2.Init LVGL core*/
    lv_init();

    // /*3.Init LVGL display*/
    //lv_disp_draw_buf_init(&disp_buf, buf0, buf1, DISP_HOR_RES * DISP_VER_RES / 2); //FIXME: Check for deletion
    disp = lv_display_create((int32_t)DISP_HOR_RES,(int32_t)DISP_VER_RES);
    lv_display_set_flush_cb(disp,disp_flush_cb);
    lv_display_set_buffers(disp,buf0,buf1,sizeof(buf0),LV_DISPLAY_RENDER_MODE_DIRECT);


#if INPUTDEV_TS
    // /*4.Init touch screen as input device*/ 
    //lv_indev_drv_init(&indev_ts); //FIXME: Check for deletion
    //indev_ts.type = LV_INDEV_TYPE_POINTER;//FIXME: Check for deletion    
    //indev_ts.read_cb = ts_read_cb;//FIXME: Check for deletion            
    lv_indev_t *ts_indev = lv_indev_create();
    lv_indev_set_type(ts_indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(ts_indev, ts_read_cb);
    //Enable touch IRQ
    DEV_IRQ_SET(Touch_INT_PIN, GPIO_IRQ_EDGE_RISE, &touch_callback);
#endif

    // /*5.Init imu as input device*/
    //lv_indev_drv_init(&indev_en); //FIXME: Check for deletion  
    //indev_en.type = LV_INDEV_TYPE_ENCODER;  //FIXME: Check for deletion
    //indev_en.read_cb = encoder_read_cb; //FIXME: Check for deletion        
    lv_indev_t * encoder_indev = lv_indev_create();
    lv_indev_set_type(encoder_indev, LV_INDEV_TYPE_ENCODER);
    lv_indev_set_read_cb(encoder_indev, encoder_read_cb);
    group = lv_group_create();
    lv_indev_set_group(encoder_indev, group);

    // /6.Init DMA for transmit color data from memory to SPI
    dma_channel_set_irq0_enabled(dma_tx, true);
    irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
    irq_set_enabled(DMA_IRQ_0, true);

}

/********************************************************************************
function:	Refresh image by transferring the color data to the SPI bus by DMA
parameter:
********************************************************************************/
static void disp_flush_cb(lv_display_t * disp, const lv_area_t * area, uint8_t * color_p)
{

    LCD_1IN28_SetWindows(area->x1, area->y1, area->x2 , area->y2);
    dma_channel_configure(dma_tx,
                          &c,
                          &spi_get_hw(LCD_SPI_PORT)->dr, 
                          color_p, // read address
                          ((area->x2 + 1 - area-> x1)*(area->y2 + 1 - area -> y1))*2,
                          true);
}


/********************************************************************************
function:   Touch interrupt handler
parameter:
********************************************************************************/
static void touch_callback(uint gpio, uint32_t events)
{
    if (gpio == Touch_INT_PIN)
    {
        CST816S_Get_Point();
        ts_x = Touch_CTS816.x_point;
        ts_y = Touch_CTS816.y_point;
        ts_act = LV_INDEV_STATE_PRESSED;
    }
}

/********************************************************************************
function:   Update touch screen input device status
parameter:
********************************************************************************/
static void ts_read_cb(lv_indev_t * drv, lv_indev_data_t*data)
{
    data->point.x = ts_x;
    data->point.y = ts_y; 
    data->state = ts_act;
    ts_act = LV_INDEV_STATE_RELEASED;
}

/********************************************************************************
function:   Sample IMU data 
parameter:
********************************************************************************/
static void get_diff_data(void)
{
    static int but_flag = 1;

    int i;
    float acc[3], gyro[3];
    unsigned int tim_count = 0;
    float ud_diff = 0;
    float lr_diff = 0;

    float offset = 450;
    float offset_x = 450; 

    for( i = 0;i < 3;i ++)
    {
      QMI8658_read_xyz(acc, gyro, &tim_count);
      ud_diff += gyro[1];
      lr_diff += gyro[0];
    }
    ud_diff = ud_diff / 3;
    lr_diff = lr_diff / 3;

    //up or down
    if((ud_diff > offset) && (ud_diff > 0) && (but_flag == 1))
    {
        encoder_diff -= 2;
        but_flag = 0;
    }
    else if((ud_diff < (0-offset)) && (ud_diff < 0) && (but_flag == 1)) 
    {
        encoder_diff += 2;
        but_flag = 0;
    }
    else 
    {
        encoder_diff = 0;
        but_flag = 1;
    }

    //left
    if((lr_diff > offset_x))
        encoder_act = LV_INDEV_STATE_PRESSED;
    else 
        encoder_act = LV_INDEV_STATE_RELEASED;
}

/********************************************************************************
function:	Update encoder input device status
parameter:
********************************************************************************/
static void encoder_read_cb(lv_indev_t * drv, lv_indev_data_t*data)
{
    data->enc_diff = encoder_diff;
    data->state    = encoder_act; 
    /* encoder_diff = 0; */
}

/********************************************************************************
function:   Indicate ready with the flushing when DMA complete transmission
parameter:
********************************************************************************/
static void dma_handler(void)
{
    if (dma_channel_get_irq0_status(dma_tx)) {
        dma_channel_acknowledge_irq0(dma_tx);
        lv_disp_flush_ready(disp);         /* Indicate you are ready with the flushing*/
    }
}


/********************************************************************************
function:   Report the elapsed time to LVGL each 5ms
parameter:
********************************************************************************/
static bool repeating_lvgl_timer_callback(struct repeating_timer *t) 
{
    lv_tick_inc(5);
    return true;
}

/********************************************************************************
function:   Update IMU label data each 500ms
parameter:
********************************************************************************/
static bool repeating_imu_data_update_timer_callback(struct repeating_timer *t) 
{
    char label_text[64];
    float acc[3], gyro[3];
    unsigned int tim_count = 0;
   
    QMI8658_read_xyz(acc, gyro, &tim_count);
    sprintf(label_text,"%4.1f \n%4.1f \n%4.1f \n\n%4.1f \n%4.1f \n%4.1f ",acc[0],acc[1],acc[2],gyro[0],gyro[1],gyro[2]);
    lv_label_set_text(label_imu,label_text);
    return true;
}

/********************************************************************************
function:   Trigger IMU data sampling each 50ms
parameter:
********************************************************************************/
static bool repeating_imu_diff_timer_callback(struct repeating_timer *t)
{
    get_diff_data();
    return true;
}


int main(void)
{
    if (DEV_Module_Init() != 0)
    {
        return -1;
    } 
    printf("LCD_1in28_LCGL_test Demo\r\n");
    // /*Init LCD*/
    LCD_1IN28_Init(HORIZONTAL);
    LCD_1IN28_Clear(WHITE);
    DEV_SET_PWM(100);
    // /*Init touch screen*/ 
    CST816S_init(CST816S_Point_Mode);
    // /*Init IMU*/
    QMI8658_init();
    // /*Init LVGL*/
    LVGL_Init();
    ui_init();
    
    while(1)
    {
      lv_task_handler();
      DEV_Delay_ms(5); 
    }

    DEV_Module_Exit();
    return 0;

    return 0;
}
/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "../lv_port/lv_port_disp.h"
#include "../lv_port/lv_port_indev.h"
#include "../lv_port/lv_port_fs.h"
#include "demos/lv_demos.h"
#include "libraries\bsp\bsp_i2c.h"
//#include "libraries\bsp\bsp_qmi8658.h"
#include "libraries\bsp\bsp_pcf85063.h"
#include "libraries\bsp\bsp_battery.h"

#include "hardware/adc.h"
#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"

#include "ui.h"
#include "AccelInterface.h"

#define LVGL_TICK_PERIOD_MS 5
#define BSP_TICK_PERIOD_MS 100


#define DISP_HOR_RES 466
#define DISP_VER_RES 466

float pitch = 0;

inline float normalize(float input)
{
    return ((10/9)*input + 50);
}

char *turnFloat2Char(float input)
{
    char buffer[4];
    //char buffer[4];
    int ret = snprintf(buffer, sizeof(buffer), "%f", input);

    if (ret < 0) {
        //return;
    }
    if (ret >= sizeof(buffer)) {
        /* Result was truncated - resize the buffer and retry.*/
    }
    char *retVal = buffer;
    return retVal;
}

/********************************************************************************
function:	Calculate Roll and pitch
parameter:
********************************************************************************/
void CalculateRP(float acc[3], float *RP)
{
    float Xbuff = acc[0];
    float Ybuff = acc[1];
    float Zbuff = acc[2];
    
    RP[0] = atan2(Ybuff , -Xbuff) * 57.3;
    RP[1] = atan2(Zbuff,-Xbuff ) * 57.3;

    _ui_label_set_property(uic_RollText,_UI_LABEL_PROPERTY_TEXT,turnFloat2Char(RP[0]));
    _ui_label_set_property(uic_PitchText,_UI_LABEL_PROPERTY_TEXT,turnFloat2Char(RP[1]));
    lv_arc_set_value(uic_RollA,(int16_t)(100-normalize(RP[0])));
    lv_arc_set_value(uic_RollB,(int16_t)(100-normalize(RP[0])));
    lv_slider_set_value(uic_Pitch,(int32_t)normalize(RP[1]), LV_ANIM_ON);
}

void set_cpu_clock(uint32_t freq_Mhz)
{
    set_sys_clock_khz(freq_Mhz * 1000, true);
    clock_configure(
        clk_peri,
        0,
        CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
        freq_Mhz * 1000 * 1000,
        freq_Mhz * 1000 * 1000);
}


static bool repeating_lvgl_timer_cb(struct repeating_timer *t)
{
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
    //for(int i = 0; i<1000;i++);
    printf("%f \n", pitch);
    
    return true;
}

static bool repeating_bsp_timer_cb(struct repeating_timer *t)
{
    //qmi8658_data_t data;
    //bsp_qmi8658_read_data(&data);
    //float RP[2];
    //float Xbuff = data.acc_x;
    //float Ybuff = data.acc_y;
    //float Zbuff = data.acc_z;
    ////printf("acc: %5d %5d %5d , gyr:%5d %5d %5d\r\n", data.acc_x, data.acc_y, data.acc_z, data.gyr_x, data.gyr_y, data.gyr_z);
    ////printf("acc: %f %f %f \r\n", data.AngleX, data.AngleY, data.AngleZ);
    //RP[0] = atan2(Ybuff , -Xbuff) * 57.3;
    //RP[1] = atan2(Zbuff,-Ybuff ) * 57.3;
    //pitch = RP[1];
    ////printf("Pitch: %f Roll %f \n",RP[0],RP[1]);
    ////RP[0] = 50;
    ////RP[1] = 50;
    //_ui_label_set_property(uic_RollText,_UI_LABEL_PROPERTY_TEXT,turnFloat2Char(data.AngleX));
    //_ui_label_set_property(uic_PitchText,_UI_LABEL_PROPERTY_TEXT,turnFloat2Char(data.AngleZ));
    //lv_arc_set_value(uic_RollA,(int16_t)(100-normalize(-data.AngleX)));
    //lv_arc_set_value(uic_RollB,(int16_t)(100-normalize(data.AngleX)));
    //lv_slider_set_value(uic_Pitch,(int32_t)normalize(pitch), LV_ANIM_ON);

    return true;
}
int main()
{
    struct tm now_tm;
    stdio_init_all();
    set_cpu_clock(250);
    adc_init();
    bsp_battery_init();
    adc_set_temp_sensor_enabled(true);
    bsp_i2c_init();
    //bsp_qmi8658_init();
    bsp_pcf85063_init();
    bsp_pcf85063_get_time(&now_tm);
    if (now_tm.tm_year < 125 || now_tm.tm_year > 130)
    {
        now_tm.tm_year = 2025 - 1900; // The year starts from 1900
        now_tm.tm_mon = 1 - 1;        // Months start from 0 (November = 10)
        now_tm.tm_mday = 1;           // Day of the month
        now_tm.tm_hour = 12;          // Hour
        now_tm.tm_min = 0;            // Minute
        now_tm.tm_sec = 0;            // Second
        now_tm.tm_isdst = -1;         // Automatically detect daylight saving time
        bsp_pcf85063_set_time(&now_tm);
    }
    lv_init();
    lv_port_fs_init();
    lv_port_disp_init(DISP_HOR_RES, DISP_VER_RES, 0, false);
    lv_port_indev_init(DISP_HOR_RES, DISP_VER_RES, 0);
    static struct repeating_timer lvgl_timer;
    static struct repeating_timer bsp_timer;
    add_repeating_timer_ms(LVGL_TICK_PERIOD_MS, repeating_lvgl_timer_cb, NULL, &lvgl_timer);
    add_repeating_timer_ms(BSP_TICK_PERIOD_MS, repeating_bsp_timer_cb, NULL, &bsp_timer); 
    // lv_demo_widgets();
    ui_init();
    while (true)
    {
        lv_timer_handler();
        sleep_ms(LVGL_TICK_PERIOD_MS);
    }
}

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
#include "hardware/watchdog.h"

#include "ui.h"
#include "AccelInterface.h"

#define LVGL_TICK_PERIOD_MS 5
#define BSP_TICK_PERIOD_MS 100


#define DISP_HOR_RES 466
#define DISP_VER_RES 466



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
    
    //static uint8_t counter = 0;
    //counter++;
    //printf("Timer Counter: %d\n", counter);//Important or LCD will crash
    return true;
}

static bool repeating_bsp_timer_cb(struct repeating_timer *t)
{
    static uint8_t counter = 0;
    AccelInterface *AccInter = AccelInterface::getInstance();
    AccelInterface::RollPitch RP = AccInter->getPitchAndRoll();
    _ui_label_set_property(uic_RollText,_UI_LABEL_PROPERTY_TEXT,turnFloat2Char(RP.roll));
    _ui_label_set_property(uic_PitchText,_UI_LABEL_PROPERTY_TEXT,turnFloat2Char(RP.pitch));
    lv_slider_set_value(uic_RollA,(int32_t)(100-normalize(-RP.roll)), LV_ANIM_ON);
    lv_slider_set_value(uic_RollB,(int32_t)(100-normalize(RP.roll)), LV_ANIM_ON);
    lv_slider_set_value(uic_Pitch,(int32_t)normalize(RP.pitch), LV_ANIM_ON);
    //printf("Roll: %f \n", RP.roll);
    //printf("Pitch: %f \n", RP.pitch);
    
    counter++;
    printf("Accel Counter: %d\n", counter);//Important or LCD will crash

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
    
    bsp_pcf85063_init();
    bsp_pcf85063_get_time(&now_tm);
    AccelInterface *AccInter = AccelInterface::getInstance();
    AccInter->initialize();
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
    if (watchdog_enable_caused_reboot()) {
        printf("Rebooted by Watchdog!\n");
        //return 0;
    } else {
        printf("Clean boot\n");
    }
    // Enable the watchdog, requiring the watchdog to be updated every 100ms or the chip will reboot
    // second arg is pause on debug which means the watchdog will pause when stepping through code
    //watchdog_enable(8000, 1);

    while (true)
    {
        lv_timer_handler();
        //    static uint8_t counter = 0;
        //    counter++;
        //    printf("Main Counter: %d\n", counter);//Important or LCD will crash
        //watchdog_update();
    }
}

// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 8.3.6
// Project name: CarTouchScreenMiniDisplay

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////

// SCREEN: ui_Inclinometer
void ui_Inclinometer_screen_init(void);
void ui_event_Inclinometer(lv_event_t * e);
lv_obj_t * ui_Inclinometer;
lv_obj_t * ui_LateralScale;
lv_obj_t * ui_Pitch;
lv_obj_t * ui_PitchText;
lv_obj_t * ui_RollText;
lv_obj_t * ui_PitchName;
lv_obj_t * ui_RollName;
lv_obj_t * ui_RollA;
lv_obj_t * ui_RollB;
// CUSTOM VARIABLES
lv_obj_t * uic_Pitch;
lv_obj_t * uic_PitchText;
lv_obj_t * uic_RollText;
lv_obj_t * uic_RollA;
lv_obj_t * uic_RollB;

// SCREEN: ui_Temperature
void ui_Temperature_screen_init(void);
void ui_event_Temperature(lv_event_t * e);
lv_obj_t * ui_Temperature;
lv_obj_t * ui_TempLabel;
lv_obj_t * ui_Label2;
// CUSTOM VARIABLES

// EVENTS
lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=1
    #error "LV_COLOR_16_SWAP should be 1 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_Inclinometer(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_Temperature, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_Temperature_screen_init);
    }
}

void ui_event_Temperature(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_Inclinometer, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_Inclinometer_screen_init);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Inclinometer_screen_init();
    ui_Temperature_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_Inclinometer);
}

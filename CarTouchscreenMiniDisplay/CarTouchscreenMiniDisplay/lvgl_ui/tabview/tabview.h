#pragma once

#include "../lvgl_ui.h"

typedef void (*wifi_mode_change_cb)(uint8_t);

extern lv_obj_t *label_sd;
extern lv_obj_t *label_chip_temp;
extern lv_obj_t *label_chip_name;
extern lv_obj_t *label_battery;
extern lv_obj_t *label_psram;
extern lv_obj_t *label_flash;
extern lv_obj_t *label_ip_address;
extern lv_obj_t *label_wifi_sta_ip;
extern lv_obj_t *label_wifi_ap_ip;


extern lv_obj_t *accel_x_label;
extern lv_obj_t *accel_y_label;
extern lv_obj_t *accel_z_label;
extern lv_obj_t *gyro_x_label;
extern lv_obj_t *gyro_y_label;
extern lv_obj_t *gyro_z_label;

extern lv_obj_t *img_camera;

void system_tab_create(lv_obj_t * tab);
void camera_tab_create(lv_obj_t * tab);
void qmi8568_tab_create(lv_obj_t * tab);

void wifi_tab_create(lv_obj_t * tab);
void wifi_set_mode_change_cb(wifi_mode_change_cb cb);
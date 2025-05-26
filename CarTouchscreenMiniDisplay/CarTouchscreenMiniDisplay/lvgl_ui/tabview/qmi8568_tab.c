
#include "tabview.h"

lv_obj_t *accel_x_label;
lv_obj_t *accel_y_label;
lv_obj_t *accel_z_label;
lv_obj_t *gyro_x_label;
lv_obj_t *gyro_y_label;
lv_obj_t *gyro_z_label;

void qmi8568_tab_create(lv_obj_t * tab)
{
    lv_obj_t *list = lv_list_create(tab);
    lv_obj_set_size(list, lv_pct(100), lv_pct(100));  // 设置列表大小

    lv_obj_t *list_item = lv_list_add_btn(list, NULL, "accel_x");
    accel_x_label = lv_label_create(list_item);
    lv_label_set_text(accel_x_label, "0.00");  // 初始值

    list_item = lv_list_add_btn(list, NULL, "accel_y");
    accel_y_label = lv_label_create(list_item);
    lv_label_set_text(accel_y_label, "0.00");  // 初始值

    list_item = lv_list_add_btn(list, NULL, "accel_z");
    accel_z_label = lv_label_create(list_item);
    lv_label_set_text(accel_z_label, "0.00");  // 初始值

    list_item = lv_list_add_btn(list, NULL, "gyro_x");
    gyro_x_label = lv_label_create(list_item);
    lv_label_set_text(gyro_x_label, "0.00");  // 初始值

    list_item = lv_list_add_btn(list, NULL, "gyro_y");
    gyro_y_label = lv_label_create(list_item);
    lv_label_set_text(gyro_y_label, "0.00");  // 初始值

    list_item = lv_list_add_btn(list, NULL, "gyro_z");
    gyro_z_label = lv_label_create(list_item);
    lv_label_set_text(gyro_z_label, "0.00");  // 初始值
}

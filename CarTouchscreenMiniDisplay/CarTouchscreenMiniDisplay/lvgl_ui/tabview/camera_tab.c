#include "tabview.h"
lv_obj_t *img_camera;
void camera_tab_create(lv_obj_t * tab)
{
    img_camera = lv_img_create(tab);
    lv_obj_set_size(img_camera, lv_pct(100), lv_pct(100));
    lv_obj_align(img_camera, LV_ALIGN_CENTER, 0, 0);  // 居中显示
}

SET(SOURCES screens/ui_Inclinometer.c
    screens/ui_Screen7.c
    screens/ui_Temperature.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_scale_drawio_png.c)

add_library(ui ${SOURCES})

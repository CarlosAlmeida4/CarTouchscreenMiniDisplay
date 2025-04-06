SET(SOURCES screens/ui_Inclinometer.c
    screens/ui_Screen7.c
    screens/ui_Temperature.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_scale_png.c
    fonts/ui_font_HeadingDegree.c
    fonts/ui_font_SmallerEmblemaOne.c)



 include_directories(../lib/Config)
 include_directories(../lib/LCD)
 include_directories(../lib/QMI8658)
 include_directories(../lib/Touch)
 include_directories(../lib/lvgl)
 include_directories(./inc)
 
 
 # 生成链接库
 add_library(ui ${SOURCES})
 target_link_libraries(ui PUBLIC Config LCD Touch QMI8658 LVGL hardware_clocks hardware_rtc hardware_adc hardware_dma hardware_spi)
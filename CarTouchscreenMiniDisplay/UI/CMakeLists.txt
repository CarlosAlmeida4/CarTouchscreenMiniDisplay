SET(SOURCES screens/ui_Screen1.c
    screens/ui_Screen2.c
    screens/ui_Screen3.c
    screens/ui_Screen6.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c)

include_directories(../lib/Config)
include_directories(../lib/LCD)
include_directories(../lib/QMI8658)
include_directories(../lib/Touch)
include_directories(../lib/lvgl)
include_directories(./inc)


# 生成链接库
add_library(ui ${SOURCES})
target_link_libraries(UI PUBLIC Config LCD Touch QMI8658 LVGL hardware_clocks hardware_rtc hardware_adc hardware_dma hardware_spi)

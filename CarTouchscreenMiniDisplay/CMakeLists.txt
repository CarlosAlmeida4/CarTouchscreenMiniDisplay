# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(CarTouchscreenMiniDisplay C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

#添加编译子目录
add_subdirectory(./lib/Config)
add_subdirectory(./lib/LCD)
add_subdirectory(./lib/Touch)
add_subdirectory(./lib/QMI8658)
add_subdirectory(./lib/lvgl)
add_subdirectory(./examples)
add_subdirectory(./UI)


#添加头文件目录
include_directories(./examples)
include_directories(./lib/Config)
include_directories(./lib/LCD)
include_directories(./lib/Touch)
include_directories(./lib/QMI8658)
include_directories(./lib/lvgl)
include_directories(./examples/inc)
include_directories(./UI)



add_executable(CarTouchscreenMiniDisplay main.c)

pico_set_program_name(CarTouchscreenMiniDisplay  "CarTouchscreenMiniDisplay")
pico_set_program_version(CarTouchscreenMiniDisplay "0.1")

pico_enable_stdio_uart(CarTouchscreenMiniDisplay 1)
pico_enable_stdio_usb(CarTouchscreenMiniDisplay  0)

# Add the standard library to the build

set(LV_CONF_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/LVGL/lv_conf.h
    CACHE STRING "" FORCE)
add_compile_options(-O2)


  target_link_libraries(CarTouchscreenMiniDisplay  
                        LCD 
                        Touch
                        QMI8658
                        LVGL
                        Config
                        examples
                        pico_stdlib
                        hardware_spi
                        hardware_i2c
                        hardware_dm
                        )
# Add the standard include files to the build
target_include_directories(CarTouchscreenMiniDisplay   PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  # ${CMAKE_CURRENT_LIST_DIR}
  )

pico_add_extra_outputs(CarTouchscreenMiniDisplay)


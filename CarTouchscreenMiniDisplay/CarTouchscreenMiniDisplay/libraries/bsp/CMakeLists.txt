# Search for all source files in the current directory
# and save the names to the DIR_Config_SRCS variable
aux_source_directory(. DIR_BSP_SRCS)

# Generate link library
add_library(bsp ${DIR_BSP_SRCS})


# Add the standard include files to the build
target_include_directories(bsp PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(bsp PUBLIC 
    pico_stdlib 
    hardware_spi 
    hardware_i2c 
    hardware_pwm 
    hardware_adc 
    hardware_dma
    hardware_pio
    hardware_irq)


if (NOT FREERTOS_KERNEL_PATH AND NOT DEFINED ENV{FREERTOS_KERNEL_PATH})
  message("Skipping FreeRTOS examples as FREERTOS_KERNEL_PATH not defined")
  return()
else ()
  target_link_libraries(bsp PUBLIC 
    FreeRTOS-Kernel-Heap4
    i2c_dma)
  target_compile_definitions(bsp PUBLIC ENABLED_FREERTOS)
endif()



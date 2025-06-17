# AccelInterface/CMakeLists.txt

add_library(AccelInterface STATIC
    AccelInterface.cpp
)


target_include_directories(AccelInterface PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(AccelInterface 
                        pico_stdlib
                        bsp)
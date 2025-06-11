# AccelInterface/CMakeLists.txt

add_library(AccelInterface STATIC
    AccelInterface.cpp
)

target_include_directories(AccelInterface PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../libraries/bsp
)

target_link_libraries(AccelInterface pico_stdlib)
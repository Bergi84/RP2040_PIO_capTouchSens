cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(capTouchSensTest C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(capTouchSensTest)

target_sources(capTouchSensTest PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        )

target_include_directories(capTouchSensTest PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(capTouchSensTest pico_stdlib hardware_pio hardware_irq)

pico_generate_pio_header(capTouchSensTest ${CMAKE_CURRENT_LIST_DIR}/capTouchSens.pio)

pico_enable_stdio_usb(capTouchSensTest 1)

pico_add_extra_outputs(capTouchSensTest)
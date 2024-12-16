cmake_minimum_required(VERSION 3.21)
if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(gpio_digital_in_out_demo LANGUAGES MikroC)
else()
    project(gpio_digital_in_out_demo LANGUAGES C ASM)
endif()

include(mikroeUtils)

add_executable(gpio_digital_in_out_demo
    digital_in_out.h
    digital_in_out.c
    main.c
)

find_package(MikroC.Core REQUIRED)
find_package(MikroSDK.Board REQUIRED)
find_package(MikroSDK.Driver.GPIO.Out REQUIRED)
find_package(MikroSDK.Driver.GPIO.In REQUIRED)

target_link_libraries(gpio_digital_in_out_demo PUBLIC
    MikroC.Core
    MikroSDK.Board
    MikroSDK.Driver.GPIO.Out
    MikroSDK.Driver.GPIO.In
)

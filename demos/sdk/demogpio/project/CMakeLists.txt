cmake_minimum_required(VERSION 3.21)

if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(gpio_demo_pack LANGUAGES MikroC)
else()
    project(gpio_demo_pack LANGUAGES C ASM)
endif()

## Port
add_subdirectory(demoport)

## LED curtain
add_subdirectory(demoledcurtain)

## LED blinking
add_subdirectory(demoledblinking)

## Read input on one pin, display output on another pin
add_subdirectory(demodigitalinputoutput)

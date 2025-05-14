cmake_minimum_required(VERSION 3.19)

project(bare_metal_motor LANGUAGES C ASM)

include(mikroeUtils)

set(PROJECT_TYPE "mikrosdk" CACHE STRING "" FORCE)

if (${_MSDK_BOARD_NAME_} STREQUAL "RA4M1_CLICKER")
    set(vendor "renesas")
else()
    message(FATAL_ERROR "This Demo only supports Bare Metal setup with the following boards:\n- RA4M1 Clicker\nPlease select a Bare Metal setup for this board.")
endif()

add_executable(bare_metal_motor
    main.c
    motor_demo.h
    ${vendor}/motor_demo.c
)

find_package(MikroC.Core REQUIRED)
target_link_libraries(bare_metal_motor
    PUBLIC
        MikroC.Core
)

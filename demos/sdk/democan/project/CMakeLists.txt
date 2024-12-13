cmake_minimum_required(VERSION 3.21)

if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(can_demo_pack LANGUAGES MikroC)
else()
    project(can_demo_pack LANGUAGES C ASM)
endif()

## Receive
add_subdirectory(receive)

## Transmit
add_subdirectory(transmit)

## Loopback
add_subdirectory(loopback)

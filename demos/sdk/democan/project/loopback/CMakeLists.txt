cmake_minimum_required(VERSION 3.21)
if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(can_loopback_demo LANGUAGES MikroC)
else()
    project(can_loopback_demo LANGUAGES C ASM)
endif()

include(mikroeUtils)

add_executable(can_loopback_demo
    main.c
)

find_package(MikroC.Core REQUIRED)
find_package(MikroSDK.Board REQUIRED)
find_package(MikroSDK.Driver.GPIO.Out REQUIRED)
find_package(MikroSDK.Driver.GPIO.Port REQUIRED)
find_package(MikroSDK.Driver.CAN REQUIRED)
find_package(MikroSDK.mikroSDKVersion REQUIRED)

if (NOT MikroSDK.Driver.CAN\_FOUND)
    message(FATAL_ERROR "FATAL ERROR: Selected setup does not have CAN.")
endif()

target_link_libraries(can_loopback_demo
    PUBLIC
        MikroC.Core
        MikroSDK.Board
        MikroSDK.Driver.GPIO.Out
        MikroSDK.Driver.GPIO.Port
        MikroSDK.Driver.CAN
        MikroSDK.mikroSDKVersion
)

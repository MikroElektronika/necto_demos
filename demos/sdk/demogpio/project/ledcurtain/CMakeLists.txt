cmake_minimum_required(VERSION 3.21)
if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(led_curtain_demo LANGUAGES MikroC)
else()
    project(led_curtain_demo LANGUAGES C ASM)
endif()

include(mikroeUtils)

set(PROJECT_TYPE "mikrosdk" CACHE STRING "" FORCE)

add_executable(led_curtain_demo
    main.c
)

find_package(MikroC.Core REQUIRED)
find_package(MikroSDK.Board REQUIRED)
find_package(MikroSDK.Driver.GPIO.Port REQUIRED)

target_link_libraries(led_curtain_demo PUBLIC
    MikroC.Core
    MikroSDK.Board
    MikroSDK.Driver.GPIO.Port
)

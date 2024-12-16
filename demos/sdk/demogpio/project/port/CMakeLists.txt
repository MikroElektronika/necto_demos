cmake_minimum_required(VERSION 3.21)
if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(port_demo LANGUAGES MikroC)
else()
    project(port_demo LANGUAGES C ASM)
endif()

include(mikroeUtils)

add_executable(port_demo
    main.c
    ports.h
)

find_package(MikroC.Core REQUIRED)
find_package(MikroSDK.Board REQUIRED)
find_package(MikroSDK.Driver.GPIO.Port REQUIRED)

if(${MCU_NAME} MATCHES "TM4C")
    target_compile_definitions(port_demo PUBLIC "TIVA")
endif()

target_link_libraries(port_demo PUBLIC
    MikroC.Core
    MikroSDK.Board
    MikroSDK.Driver.GPIO.Port
)

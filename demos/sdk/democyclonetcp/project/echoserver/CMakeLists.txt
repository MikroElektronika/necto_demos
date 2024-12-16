cmake_minimum_required(VERSION 3.21)
if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    message(FATAL_ERROR "Ethernet examples are only meant to be used with GCC")
else()
    project(demo_ethernet_echo_server LANGUAGES C ASM)
endif()

include(mikroeUtils)

if (NOT ${TOOLCHAIN_ID} STREQUAL "mchp_xc32")
    add_executable(demo_ethernet_echo_server
        main.c
        systick.h
    )
    add_compile_definitions(SYSTICK_PRESENT)
else()
    add_executable(demo_ethernet_echo_server
        main.c
        timer.h
    )
    target_link_options(demo_ethernet_echo_server PRIVATE "LINKER:--defsym=_min_heap_size=16384")
    if (${MCU_NAME} MATCHES "PIC32MX")
        add_compile_definitions(PIC32MX)
    elseif (${MCU_NAME} MATCHES "PIC32MZ")
        add_compile_definitions(PIC32MZ)
    endif()
endif()

target_compile_definitions(demo_ethernet_echo_server
    PUBLIC
        "MCU_NAME_CMAKE=\"${MCU_NAME}\""
)

find_package(MikroC.Core REQUIRED)
find_package(MikroSDK.Log REQUIRED)
find_package(MikroSDK.Board REQUIRED)
find_package(MikroSDK.Driver.GPIO.Out REQUIRED)
find_package(MikroSDK.Ethernet.CycloneTCP REQUIRED)

if (NOTMikroSDK.Ethernet.CycloneTCP\_FOUND)
    message(FATAL_ERROR "FATAL ERROR: Selected setup does not have Ethernet.")
endif()

target_link_libraries(demo_ethernet_echo_server PUBLIC
    MikroC.Core
    MikroSDK.Log
    MikroSDK.Board
    MikroSDK.Driver.GPIO.Out
    MikroSDK.Ethernet.CycloneTCP
)

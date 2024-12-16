cmake_minimum_required(VERSION 3.21)
if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    message(FATAL_ERROR "FATAL ERROR: USB examples are only meant to be used with GCC")
else()
    project(usb_cdc_msc_demo LANGUAGES C ASM)
endif()

include(mikroeUtils)

add_executable(usb_cdc_msc_demo
    main.c
    usb_descriptors.c
    msc_disk.c
)

find_package(MikroC.Core REQUIRED)
find_package(MikroSDK.USB.Tiny REQUIRED)
if (NOT MikroSDK.USB.Tiny\_FOUND)
    message(FATAL_ERROR "FATAL ERROR: Selected setup does not have USB.")
endif()

target_link_libraries(usb_cdc_msc_demo PUBLIC
    MikroC.Core
    MikroSDK.USB.Tiny
)

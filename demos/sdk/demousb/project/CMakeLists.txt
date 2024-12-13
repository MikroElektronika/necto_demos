cmake_minimum_required(VERSION 3.21)

if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    message(FATAL_ERROR "MikroC not supported for this demo.")
else()
    project(usb_demo_pack LANGUAGES C ASM)
endif()

## HID
add_subdirectory(hid)

## CDC
add_subdirectory(cdc)

## MIDI
add_subdirectory(midi)

cmake_minimum_required(VERSION 3.21)

if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    message(FATAL_ERROR "MikroC not supported for this demo.")
else()
    project(usb_demo_pack LANGUAGES C ASM)
endif()

include(mikroeUtils)

## HID
add_subdirectory(hid)

## CDC
add_subdirectory(cdc)

## MIDI
add_subdirectory(midi)

if(COMPILER_REVISION)
    if(${COMPILER_REVISION} VERSION_GREATER_EQUAL "1.0")
        if(${TOOLCHAIN_LANGUAGE} STREQUAL "GNU")
            if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.meproject)
                fetch_startup_name(${CMAKE_CURRENT_SOURCE_DIR}/.meproject startupName)
                if(startupName)
                    add_startup_all_targets(. ${CMAKE_CURRENT_SOURCE_DIR} ${startupName})
                endif()
                fetch_linker_name(${CMAKE_CURRENT_SOURCE_DIR}/.meproject linkerName)
                if(linkerName)
                    add_ld_all_targets(. ${CMAKE_CURRENT_SOURCE_DIR} ${linkerName})
                endif()
            endif()
        endif()
    endif()
endif()

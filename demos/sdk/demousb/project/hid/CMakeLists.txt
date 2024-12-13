cmake_minimum_required(VERSION 3.21)
if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    message(FATAL_ERROR "USB examples are only meant to be used with GCC")
else()
    project(usb_hid_generic_inout_demo LANGUAGES C ASM)
endif()

include(mikroeUtils)

add_executable(usb_hid_generic_inout_demo
    main.c
    usb_descriptors.c
)

find_package(MikroC.Core REQUIRED)
find_package(MikroSDK.USB.Tiny REQUIRED)
if (NOT MikroSDK.USB.Tiny\_FOUND)
    message(FATAL_ERROR "FATAL ERROR: Selected setup does not have USB.")
endif()

target_link_libraries(usb_hid_generic_inout_demo PUBLIC
    MikroC.Core
    MikroSDK.USB.Tiny
)

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

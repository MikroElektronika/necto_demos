cmake_minimum_required(VERSION 3.21)
if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(can_transmit_demo LANGUAGES MikroC)
else()
    project(can_transmit_demo LANGUAGES C ASM)
endif()

include(mikroeUtils)

add_executable(can_transmit_demo
    main.c
)
find_package(MikroSDK.Driver.GPIO.Port REQUIRED)
find_package(MikroSDK.mikroSDKVersion REQUIRED)
find_package(MikroSDK.Driver.GPIO.Out REQUIRED)
find_package(MikroSDK.Driver.GPIO.In REQUIRED)
find_package(MikroSDK.Driver.CAN REQUIRED)
find_package(MikroSDK.Driver REQUIRED)
find_package(MikroSDK.Board REQUIRED)
find_package(MikroC.Core REQUIRED)

if (NOT MikroSDK.Driver.CAN\_FOUND)
    message(FATAL_ERROR "FATAL ERROR: Selected setup does not have CAN.")
endif()

target_link_libraries(can_transmit_demo
    PUBLIC
        MikroC.Core
        MikroSDK.Board
        MikroSDK.Driver.GPIO.Port
        MikroSDK.mikroSDKVersion
        MikroSDK.Driver.GPIO.Out
        MikroSDK.Driver.GPIO.In
        MikroSDK.Driver.CAN
        MikroSDK.Driver
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

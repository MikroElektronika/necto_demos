cmake_minimum_required(VERSION 3.21)
if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(can_receive_demo LANGUAGES MikroC)
else()
    project(can_receive_demo LANGUAGES C ASM)
endif()

include(mikroeUtils)

add_executable(can_receive_demo
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

# PIC MCUs can use only FIFO0 for receiving.
if(${MCU_NAME} MATCHES "^PIC18.+$")
    target_compile_definitions(can_receive_demo PRIVATE CAN_FILTER_FIFO_NUM=CAN_FILTER_FIFO0)
# TIVA MCUs don't have FIFO0 and can use only FIFO2 for receiving as FIFO1 is used for transmitting.
elseif(${MCU_NAME} MATCHES "^TM4C.+$")
    target_compile_definitions(can_receive_demo PRIVATE CAN_FILTER_FIFO_NUM=CAN_FILTER_FIFO2)
else()
    target_compile_definitions(can_receive_demo PRIVATE CAN_FILTER_FIFO_NUM=CAN_FILTER_FIFO1)
endif()

target_link_libraries(can_receive_demo
    PUBLIC
        MikroC.Core
        MikroSDK.Board
        MikroSDK.Driver.GPIO.Out
        MikroSDK.Driver.GPIO.Port
        MikroSDK.Driver.CAN
        MikroSDK.mikroSDKVersion
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

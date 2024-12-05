cmake_minimum_required(VERSION 3.21)
if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(lcd_demo LANGUAGES MikroC)
else()
    project(lcd_demo LANGUAGES C ASM)
endif()

include(mikroeUtils)

add_executable(lcd_demo
    main.c
)

if (${MCU_NAME} MATCHES "^STM32.+")
    target_compile_definitions(lcd_demo PUBLIC
        DELAY_LCD=400
    )
endif()
if (${MCU_NAME} MATCHES "^MK.+")
    target_compile_definitions(lcd_demo PUBLIC
        DELAY_LCD=100
    )
endif()
if (${MCU_NAME} MATCHES "^TM4.+")
    target_compile_definitions(lcd_demo PUBLIC
        DELAY_LCD=200
    )
endif()
if (${MCU_NAME} MATCHES "^PIC32.+")
    target_compile_definitions(lcd_demo PUBLIC
        DELAY_LCD=200
    )
endif()
if (${MCU_NAME} MATCHES "^PIC18.+")
    target_compile_definitions(lcd_demo PUBLIC
        DELAY_LCD=250
    )
endif()
if (${CORE_NAME} MATCHES "DSPIC")
    target_compile_definitions(lcd_demo PUBLIC
        DELAY_LCD=400
    )
endif()
if (${CORE_NAME} MATCHES "RISCV")
    target_compile_definitions(lcd_demo PUBLIC
        DELAY_LCD=100
    )
endif()
if ((${CORE_NAME} MATCHES "GT64K") OR (${CORE_NAME} MATCHES "LTE64K"))
    target_compile_definitions(lcd_demo PUBLIC
        DELAY_LCD=200
    )
endif()

find_package(MikroC.Core REQUIRED)
find_package(MikroSDK.Board REQUIRED)
find_package(MikroSDK.Driver.GPIO.Out REQUIRED)
find_package(MikroSDK.LCD REQUIRED)

target_link_libraries(lcd_demo PUBLIC
    MikroC.Core
    MikroSDK.Board
    MikroSDK.Driver.GPIO.Out
    MikroSDK.LCD
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

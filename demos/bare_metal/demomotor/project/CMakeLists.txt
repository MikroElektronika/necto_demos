cmake_minimum_required(VERSION 3.19)

project(motor_demo LANGUAGES C ASM)

include(mikroeUtils)

set(PROJECT_TYPE "mikrosdk" CACHE STRING "" FORCE)

if (${_MSDK_BOARD_NAME_} STREQUAL "RA4M1_CLICKER")
    set(vendor renesas)
else()
    message(FATAL_ERROR "This Demo only supports Bare Metal setup with the following boards:\n- RA4M1 Clicker\nPlease select a setup for this board.")
endif()

add_executable(motor_demo
    main.c
    motor_demo.h
    ${vendor}/motor_demo.c
)

find_package(MikroC.Core REQUIRED)
target_link_libraries(motor_demo
    PUBLIC
        MikroC.Core
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
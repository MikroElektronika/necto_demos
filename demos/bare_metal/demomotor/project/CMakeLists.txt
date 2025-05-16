cmake_minimum_required(VERSION 3.19)

project(motor_demo LANGUAGES C ASM)

include(mikroeUtils)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
include(demomotorutils)

set(PROJECT_TYPE "mikrosdk" CACHE STRING "" FORCE)

# Choose the board and vendor for this example
define_vendor_and_board(vendor board)

add_executable(motor_demo
    main.c
    motor_demo.h
    ${vendor}/${board}.c
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
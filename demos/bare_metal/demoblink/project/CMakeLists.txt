cmake_minimum_required(VERSION 3.19)

if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(bare_metal_blinking_demo LANGUAGES MikroC)
else()
    project(bare_metal_blinking_demo LANGUAGES C ASM)
endif()

include(mikroeUtils)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
include(demoblinkingUtils)

set(PROJECT_TYPE "mikrosdk" CACHE STRING "" FORCE)

# Choose the board and vendor for this example
define_vendor_and_board(vendor board)

add_executable(bare_metal_blinking_demo
    main.c
    ${vendor}/${board}/bm_board.h
    ${vendor}/blinking.c
)

target_include_directories(bare_metal_blinking_demo
    PUBLIC
        ${vendor}/${board}
)

find_package(MikroC.Core REQUIRED)
target_link_libraries(bare_metal_blinking_demo
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

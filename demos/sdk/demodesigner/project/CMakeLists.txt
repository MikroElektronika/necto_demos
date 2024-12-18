    cmake_minimum_required(VERSION 3.21)
if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(Designer_Tool_Demo LANGUAGES MikroC)
else()
    project(Designer_Tool_Demo  LANGUAGES C ASM)
endif()

include(mikroeUtils)

set(PROJECT_TYPE "designer" CACHE STRING "" FORCE)

add_executable(Designer_Tool_Demo
    main.c
    main_screen.c
    main_screen.h
    main_screen.mscr
    second_screen.h
    second_screen.c
    second_screen.mscr
)

designer_generator(Designer_Tool_Demo resource.mres main_screen.mscr second_screen.mscr)
############################ Designer_Tool_Demo GENERATED CODE START ###########################
############################ Designer_Tool_Demo GENERATED CODE END ###########################

find_package(MikroC.Core REQUIRED)
find_package_me(MikroSDK.TouchController REQUIRED)
find_package_me(MikroSDK.GraphicLibrary REQUIRED)
find_package_me(MikroSDK.VisualTft REQUIRED)

if(${MCU_NAME} MATCHES "STM32|TM4|MK|PIC32")
## 256K Flash and 16K RAM minimum for this demo.
has_enough_memory(ENOUGH_MEMORY RAM 16384 FLASH 262144)
    if(NOT ${ENOUGH_MEMORY})
        message(FATAL_ERROR "${MCU_NAME} does not have enough memory for Designer demo.")
    endif()
else()
    message(FATAL_ERROR "This Demo is not meant for the chosen MCU architecture.")
endif()

if(NOT DEFINED IS_PD_SETUP)
    set(IS_PD_SETUP 0)
else()
    if(${IS_PD_SETUP})
        set(IS_PD_SETUP 1)
    else()
        set(IS_PD_SETUP 0)
    endif()
endif()

target_compile_definitions(Designer_Tool_Demo PUBLIC
    ${tim_list}
    IS_PD_SETUP=${IS_PD_SETUP}
    TFT_WIDTH=${_MSDK_TFT_WIDTH_}
)

target_link_libraries(Designer_Tool_Demo
    PUBLIC
        MikroC.Core
        MikroSDK.TouchController
        MikroSDK.GraphicLibrary
        MikroSDK.VisualTft
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

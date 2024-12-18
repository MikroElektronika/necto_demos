cmake_minimum_required(VERSION 3.19)

if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(demo_lvgl_keyboard LANGUAGES MikroC)
else()
    project(demo_lvgl_keyboard LANGUAGES C ASM)
endif()

include(mikroeUtils)
include(${CMAKE_CURRENT_SOURCE_DIR}/1ms_timer/TimerHeaderConfig.cmake)

set(PROJECT_TYPE "lvgl_designer" CACHE STRING "" FORCE)

add_executable(demo_lvgl_keyboard
    driver/display_lvgl.h
    driver/lv_port_indev.h
    driver/display_lvgl.c
    driver/lv_port_indev.c
    1ms_Timer.h
    resource.mres
    main_screen.mscr
    main_screen.c
    main_screen.h
    main.c
    second_screen.h
    second_screen.c
    second_screen.mscr
)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS resource.mres)

find_package(MikroC.Core REQUIRED)
find_package_me(MikroSDK.LVGL.Common REQUIRED)
find_package_me(MikroSDK.GraphicLibrary REQUIRED)
find_package_me(MikroSDK.LVGL)

lvgl_designer_generator(demo_lvgl_keyboard resource.mres main_screen.mscr second_screen.mscr)

target_link_libraries(demo_lvgl_keyboard
    PUBLIC
        MikroC.Core
        MikroSDK.LVGL
        MikroSDK.LVGL.Common
        MikroSDK.GraphicLibrary
)

if(NOT DEFINED IS_PD_SETUP)
    set(IS_PD_SETUP 0)
else()
    if(${IS_PD_SETUP})
        set(IS_PD_SETUP 1)
    else()
        set(IS_PD_SETUP 0)
    endif()
endif()

## TODO - temporary fix - remove later
has_enough_memory(ENOUGH_MEMORY RAM 131072 FLASH 524288)
if(${ENOUGH_MEMORY})
    target_compile_definitions(demo_lvgl_keyboard PUBLIC
        ${tim_list}
        IS_PD_SETUP=${IS_PD_SETUP}
        TFT_WIDTH=${_MSDK_TFT_WIDTH_}
        undef LV_LVGL_H_INCLUDE_SIMPLE
    )
else()
    target_compile_definitions(demo_lvgl_keyboard PUBLIC
        ${tim_list}
        IS_PD_SETUP=${IS_PD_SETUP}
        TFT_WIDTH=${_MSDK_TFT_WIDTH_}
    )
endif()

target_compile_definitions(demo_lvgl_keyboard PUBLIC
    ${tim_list}
    IS_PD_SETUP=${IS_PD_SETUP}
    TFT_WIDTH=${_MSDK_TFT_WIDTH_}
    ${UNDEF_FIX}
)

target_include_directories(demo_lvgl_keyboard
    PUBLIC
        driver
        1ms_timer
        .
)

if((${MCU_FLASH} LESS 184320) OR (${MCU_RAM} LESS 24576)) ## 180KB Flash / 24KB RAM recommended.
    message(FATAL_ERROR ": ${MCU_NAME} does not have recommended memory for this example.")
endif()

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

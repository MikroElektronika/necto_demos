cmake_minimum_required(VERSION 3.21)
if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(platform_plot_demo LANGUAGES MikroC)
else()
    project(platform_plot_demo LANGUAGES C ASM)
endif()

include(mikroeUtils)

add_executable(platform_plot_demo
    main.c
)

find_package(MikroSDK.Plot REQUIRED)
target_link_libraries(platform_plot_demo PUBLIC MikroSDK.Plot)
find_package(MikroC.Core REQUIRED)
target_link_libraries(platform_plot_demo PUBLIC MikroC.Core)

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

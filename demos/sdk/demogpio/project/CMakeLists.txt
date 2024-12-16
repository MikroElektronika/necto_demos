cmake_minimum_required(VERSION 3.21)

if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(gpio_demo_pack LANGUAGES MikroC)
else()
    project(gpio_demo_pack LANGUAGES C ASM)
endif()

include(mikroeUtils)

## Port
add_subdirectory(port)

## LED curtain
add_subdirectory(ledcurtain)

## LED blinking
add_subdirectory(ledblinking)

## Read input on one pin, display output on another pin
add_subdirectory(digitalinputoutput)

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

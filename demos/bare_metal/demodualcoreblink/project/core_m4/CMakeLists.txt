add_executable(core_m4 main.c)

set(CORE_SPECIFIC_SETUP_DIR ${CMAKE_SOURCE_DIR}/.meproject/setup/Debug/core_m4)
list(APPEND CMAKE_PREFIX_PATH ${CORE_SPECIFIC_SETUP_DIR}/lib/cmake)
list(APPEND CMAKE_MODULE_PATH ${CORE_SPECIFIC_SETUP_DIR}/lib/cmake)

target_compile_options(core_m4 PRIVATE "-mcpu=cortex-m4")
target_compile_definitions(core_m4 PRIVATE CORE_CM4)
set(SECONDARY_CORE "core_m4" CACHE STRING "Secondary core selection" FORCE)


find_package(MikroC.Core REQUIRED)
target_link_libraries(core_m4
    PUBLIC
        MikroC.Core
)

if(COMPILER_REVISION)
    if(${COMPILER_REVISION} VERSION_GREATER_EQUAL "1.0")
        if(${TOOLCHAIN_LANGUAGE} STREQUAL "GNU")
            if(EXISTS ${CORE_SPECIFIC_SETUP_DIR}/FileStartup)

                fetch_startup_name(${CORE_SPECIFIC_SETUP_DIR}/FileStartup startupName)
                if(startupName)
                    add_startup_all_targets(. ${CORE_SPECIFIC_SETUP_DIR}/FileStartup ${startupName})
                endif()
                fetch_linker_name(${CORE_SPECIFIC_SETUP_DIR}/FileLinker linkerName)
                if(linkerName)
                    add_ld_all_targets(. ${CORE_SPECIFIC_SETUP_DIR}/FileLinker ${linkerName})
                endif()
            endif()
        endif()
    endif()
endif()

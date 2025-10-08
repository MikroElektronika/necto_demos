cmake_minimum_required(VERSION 3.19)

if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(Dual_Core_Blink_Demo LANGUAGES MikroC)
else()
    project(Dual_Core_Blink_Demo LANGUAGES C ASM)
endif()

include(mikroeUtils)

set(PROJECT_TYPE "dual_core" CACHE STRING "" FORCE)
add_subdirectory(core_m7)
add_subdirectory(core_m4)

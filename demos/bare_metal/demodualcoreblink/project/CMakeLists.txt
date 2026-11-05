cmake_minimum_required(VERSION 3.19)

if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(Dual_Core_Blink_Demo LANGUAGES MikroC)
else()
    project(Dual_Core_Blink_Demo LANGUAGES C ASM)
endif()

include(mikroeUtils)

# This demo is written specifically for the dual core STM32H755ZI MCU.
if(NOT "${MCU_NAME}" MATCHES "^STM32H755ZI")
    message(FATAL_ERROR "Dual Core Blink Demo supports only STM32H755ZI MCU (selected MCU: '${MCU_NAME}').")
endif()

set(PROJECT_TYPE "dual_core" CACHE STRING "" FORCE)
add_subdirectory(core_m7)
add_subdirectory(core_m4)

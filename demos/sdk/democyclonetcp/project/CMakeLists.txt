cmake_minimum_required(VERSION 3.21)

if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    message(FATAL_ERROR "MikroC not supported for this demo.")
else()
    project(cyclonetcp_demo_pack LANGUAGES C ASM)
endif()

## FTP CLIENT
add_subdirectory(ftpclient)

## ECHO SERVER
add_subdirectory(echoserver)

## HTTP CLIENT
add_subdirectory(httpclient)

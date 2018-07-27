cmake_minimum_required(VERSION 2.8)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set (FB_PLATFORM_NAME "windows")
elseif(APPLE)
    set (FB_PLATFORM_NAME "macos")
elseif(UNIX)
    set (FB_PLATFORM_NAME "x11")
endif()

set(PROJNAME DiveboardAgent)
message("Building ${PROJNAME} for ${FB_PLATFORM_NAME}")

macro (include_platform)
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/platforms/${FB_PLATFORM_NAME}/projectDef.cmake)
        include(${CMAKE_CURRENT_SOURCE_DIR}/platforms/${FB_PLATFORM_NAME}/projectDef.cmake)
    else()
        message ("Could not find a ${FB_PLATFORM_NAME} directory in ${CMAKE_CURRENT_SOURCE_DIR}/platforms")
    endif()
endmacro(include_platform)

project(${PROJNAME})

file (GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    source/[^.]*.cpp
    source/[^.]*.h
    source/[^.]*.cmake
    )

set(FIND_LIBRARY_USE_LIB64_PATHS FALSE)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_platform()

# Mac template platform definition CMake file
# Included from ../CMakeLists.txt

# remember that the current source dir is the project root; this file is in Mac/
file (GLOB PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    Win/[^.]*.cpp
    Win/[^.]*.h
    Win/[^.]*.cmake
    )

# use this to add preprocessor definitions
add_definitions(
    
)


SOURCE_GROUP(Win FILES ${PLATFORM})

set (SOURCES
    ${SOURCES}
    ${PLATFORM}
    )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x -g -O0")

set(3d_paryt_ROOT "${CMAKE_SOURCE_DIR}/3d-party/lib-32")
set(wxWidgets_CONFIG_EXECUTABLE "${3d_paryt_ROOT}/wxWidgets-3.0/bin/wx-config")


set(CMAKE_PREFIX_PATH "${3d_paryt_ROOT}")

find_package( Curl REQUIRED)
include_directories(${Curl_INCLUDE_DIR})
link_directories(${Curl_LIBRARY_DIRS})

find_library(LIBICONV iconv)



find_package(wxWidgets COMPONENTS core base gl adv html xml xrc aui webview REQUIRED)
include("${wxWidgets_USE_FILE}")

set(BOOST_ROOT ${3d_paryt_ROOT})
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package( Boost COMPONENTS thread system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

message("Using boost at: ${Boost_INCLUDE_DIRS}")

find_library(LIBDIVECOMPUTER divecomputer ${3d_paryt_ROOT}/lib/ NO_DEFAULT_PATH)
find_library(LIBCONFIG config++ ${3d_paryt_ROOT}/lib/ NO_DEFAULT_PATH)
include_directories("${3d_paryt_ROOT}/include")

message("fixing include directoryies")
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
# make replace twice becouse of cmake bug
# CMAKE_MATCH_1 is set after replace and not set at execution of replace
  string(REGEX REPLACE "^/(.)/" "${CMAKE_MATCH_1}:/" dir_native ${dir})
  string(REGEX REPLACE "^/(.)/" "${CMAKE_MATCH_1}:/" dir_native ${dir})
  if(dir STREQUAL dir_native)
    message("")
  else()
    message("add ${dir_native} for ${dir}")
    include_directories("${dir_native}")
  endif()
endforeach()


add_executable(${PROJNAME} ${SOURCES})

target_link_libraries(${PROJNAME}
    ${wxWidgets_LIBRARIES}
    ${LIBDIVECOMPUTER}
    ${Boost_LIBRARIES}
    ${LIBICONV}
    ${LIBCONFIG}
    curl
    userenv.lib
    crypt32.lib
    )
    
add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND 
${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/3d-party/lib-32/bin/libconfig-9.dll" "${CMAKE_CURRENT_BINARY_DIR}/bin/$<CONFIGURATION>/libconfig-9.dll")

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND 
${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/3d-party/lib-32/bin/libconfig++-9.dll" "${CMAKE_CURRENT_BINARY_DIR}/bin/$<CONFIGURATION>/libconfig++-9.dll")

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND 
${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/3d-party/lib-32/bin/libdivecomputer-0.dll" "${CMAKE_CURRENT_BINARY_DIR}/bin/$<CONFIGURATION>/libdivecomputer-0.dll")

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND 
${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/3d-party/lib-32/bin/libcurl-4.dll" "${CMAKE_CURRENT_BINARY_DIR}/bin/$<CONFIGURATION>/libcurl-4.dll")

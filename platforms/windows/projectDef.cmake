# Windows platform definition CMake file
# Included from ../../CMakeLists.txt

# remember that the current source dir is the project root
file (GLOB PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/[^.]*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/[^.]*.h
    ${CMAKE_CURRENT_LIST_DIR}/[^.]*.cmake
    )

# use this to add preprocessor definitions
add_definitions(
    
)


SOURCE_GROUP(Win FILES ${PLATFORM})

set (SOURCES
    ${SOURCES}
    ${PLATFORM}
    )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(3rd_party_ROOT "${CMAKE_SOURCE_DIR}/3rd-party/lib-32")
set(wxWidgets_CONFIG_EXECUTABLE "${3rd_party_ROOT}/wxWidgets-3.1/bin/wx-config")


set(wxWidgets_ROOT_DIR "${3rd_party_ROOT}/wxWidgets-3.1/")
set(wxWidgets_LIB_DIR "${3rd_party_ROOT}/wxWidgets-3.1/lib" CACHE PATH "wxWidgets_LIB_DIR ")

find_package(wxWidgets COMPONENTS core base gl adv html xml xrc aui webview REQUIRED)
include("${wxWidgets_USE_FILE}")

set(BOOST_ROOT "${3rd_party_ROOT}")

# set(Boost_INCLUDE_DIRS "${3rd_party_ROOT}/include/")
# set(Boost_LIBRARY_DIRS "${3rd_party_ROOT}/lib/")
# set(Boost_INCLUDE_DIR "${3rd_party_ROOT}/include/")
# set(Boost_LIBRARY_DIR "${3rd_party_ROOT}/lib/")

set(BOOST_INCLUDEDIR "${3rd_party_ROOT}/include/")
set(BOOST_LIBRARYDIR "${3rd_party_ROOT}/lib/")
#message("Using boost at: ${Boost_INCLUDE_DIRS}")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
# find_package( Boost COMPONENTS thread REQUIRED) 
find_package( Boost COMPONENTS thread system REQUIRED) 
# include_directories(${Boost_INCLUDE_DIRS})
# link_directories(${Boost_LIBRARY_DIRS})

include_directories(${BOOST_INCLUDEDIR})
link_directories(${BOOST_LIBRARYDIR})

message("Using boost at: ${Boost_INCLUDE_DIRS}")

# find_library(LIBDIVECOMPUTER divecomputer ${3rd_party_ROOT}/lib/ NO_DEFAULT_PATH)
# find_library(LIBCONFIG config++ ${3rd_party_ROOT}/lib/ NO_DEFAULT_PATH)
set(CMAKE_PREFIX_PATH "${3rd_party_ROOT}")
# set(Curl_DIR "${3rd_party_ROOT}")
set(Curl_INCLUDE_DIR "${3rd_party_ROOT}/include/curl")
# set(Curl_LIBRARY_DIRS "${3rd_party_ROOT}/lib")

# #find_package( Curl REQUIRED)
include_directories(${Curl_INCLUDE_DIR})
# link_directories(${Curl_LIBRARY_DIRS})
find_library(LIBDIVECOMPUTER divecomputer)
find_library(LIBCONFIG config++)
find_library(LIBICONV iconv)
find_library(LIBCURL curl)

include_directories("${3rd_party_ROOT}/include")

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

# This sets up the exe icon for windows under mingw.
set(RES_FILES "")
set(RES_FILES "${CMAKE_CURRENT_LIST_DIR}/client.rc")
set(CMAKE_RC_COMPILER_INIT windres)
ENABLE_LANGUAGE(RC)
SET(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")


add_executable(${PROJNAME} ${SOURCES} ${RES_FILES})

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
${CMAKE_COMMAND} -E copy "${3rd_party_ROOT}/bin/libconfig-9.dll" "${CMAKE_CURRENT_BINARY_DIR}/bin/libconfig-9.dll")

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND 
${CMAKE_COMMAND} -E copy "${3rd_party_ROOT}/bin/libconfig++-9.dll" "${CMAKE_CURRENT_BINARY_DIR}/bin/libconfig++-9.dll")

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND 
${CMAKE_COMMAND} -E copy "${3rd_party_ROOT}/bin/libdivecomputer-0.dll" "${CMAKE_CURRENT_BINARY_DIR}/bin/libdivecomputer-0.dll")

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND 
${CMAKE_COMMAND} -E copy "${3rd_party_ROOT}/bin/libcurl-4.dll" "${CMAKE_CURRENT_BINARY_DIR}/bin/libcurl-4.dll")

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND 
${CMAKE_COMMAND} -E copy "${3rd_party_ROOT}/lib/libssl.dll.a" "${CMAKE_CURRENT_BINARY_DIR}/bin/libssl.dll.a")

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND 
${CMAKE_COMMAND} -E rename "${CMAKE_CURRENT_BINARY_DIR}/bin/libssl.dll.a" "${CMAKE_CURRENT_BINARY_DIR}/bin/libssl.dll")

# add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND 
# ${CMAKE_COMMAND} -E copy "${3rd_party_ROOT}/bin/openssl.exe" "${CMAKE_CURRENT_BINARY_DIR}/bin/openssl.exe")

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND 
${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/3rd-party/windows-build/libgcc_s_sjlj-1.dll" "${CMAKE_CURRENT_BINARY_DIR}/bin/libgcc_s_sjlj-1.dll")

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND 
${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/3rd-party/windows-build/libwinpthread-1.dll" "${CMAKE_CURRENT_BINARY_DIR}/bin/libwinpthread-1.dll")

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND 
${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/3rd-party/windows-build/libusb-1.0.dll" "${CMAKE_CURRENT_BINARY_DIR}/bin/libusb-1.0.dll")

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND 
${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/3rd-party/windows-build/libstdc++-6.dll" "${CMAKE_CURRENT_BINARY_DIR}/bin/libstdc++-6.dll")

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND 
${CMAKE_COMMAND} -E copy "${3rd_party_ROOT}/bin/libeay32.dll" "${CMAKE_CURRENT_BINARY_DIR}/bin/libeay32.dll")

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND 
${CMAKE_COMMAND} -E copy "${3rd_party_ROOT}/bin/ssleay32.dll" "${CMAKE_CURRENT_BINARY_DIR}/bin/ssleay32.dll")

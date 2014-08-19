# X11 template platform definition CMake file
# Included from ../CMakeLists.txt

# remember that the current source dir is the project root; this file is in ${PLATFORM_NAME}/
file (GLOB PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    X11/[^.]*.cpp
    X11/[^.]*.h
    X11/[^.]*.cmake
    )

SOURCE_GROUP(Lin FILES ${PLATFORM})

# use this to add preprocessor definitions
add_definitions(
)

set (SOURCES
    ${SOURCES}
    ${PLATFORM}
    )
    
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -fpermissive")
#add_x11_plugin(${PROJNAME} SOURCES)

#set(3d_party_ROOT "./3d-party/lib-32")
set(3d_party_ROOT "${CMAKE_SOURCE_DIR}/3d-party/lib-32")
set(wxWidgets_CONFIG_EXECUTABLE "${3d_party_ROOT}/wxWidgets-3.0/bin/wx-config")

set(wxWidgets_ROOT_DIR "${3d_party_ROOT}/wxWidgets-3.0/")
set(wxWidgets_LIB_DIR "${3d_party_ROOT}/wxWidgets-3.0/lib")

#install the .desktop file
INSTALL(FILES ${CMAKE_SOURCE_DIR}/${FB_PLATFORM_NAME}/${PROJNAME}.desktop DESTINATION /usr/share/applications)
message("Installed .desktop file on ${CMAKE_INSTALL_PREFIX}/share/applications")

#install the icon
INSTALL(FILES ${CMAKE_SOURCE_DIR}/forms/icon_ellow.png DESTINATION /usr/share/pixmaps)
message("Installed icon file on ${CMAKE_INSTALL_PREFIX}/share/pixmaps")


find_package(wxWidgets COMPONENTS core base gl adv html xml xrc aui webview REQUIRED)
include("${wxWidgets_USE_FILE}")

set(BOOST_ROOT "${3d_party_ROOT}")
set(BOOST_INCLUDEDIRS "${3d_party_ROOT}/include/")
set(BOOST_LIBRARYDIRS "${3d_party_ROOT}/lib/")
set(BOOST_INCLUDEDIR "${3d_party_ROOT}/include/")
set(BOOST_LIBRARYDIR "${3d_party_ROOT}/lib/")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package( Boost COMPONENTS thread system REQUIRED) 
include_directories(${BOOST_INCLUDEDIR})
include_directories($ {GTK_INCLUDE_DIRS})
link_directories(${BOOST_LIBRARYDIR})

message("Using boost at: ${Boost_INCLUDE_DIRS}")

set(CMAKE_PREFIX_PATH "${3d_party_ROOT}")
set(Curl_INCLUDE_DIR "${3d_party_ROOT}/include/curl")

include_directories(${Curl_INCLUDE_DIR})
find_library(LIBDIVECOMPUTER divecomputer)
find_library(LIBCONFIG config++)
find_library(LIBICONV iconv)
find_library(LIBCURL curl)

include_directories("${3d_party_ROOT}/include")

message("FIXING include directoryies")
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


add_executable(${PROJNAME} ${SOURCES} ${RES_FILES})

set(EXECUTABLE_NAME ${PROJNAME})
message("-- ${CMAKE_INSTALL_PREFIX}")


# add library dependencies here; leave ${PLUGIN_INTERNAL_DEPS} there unless you know what you're doing!
target_link_libraries(${PROJNAME}
    ${PLUGIN_INTERNAL_DEPS}
    ${wxWidgets_LIBRARIES}
    ${LIBDIVECOMPUTER}
    ${Boost_LIBRARIES}
    ${LIBICONV}
    ${LIBCONFIG}
    curl
    )
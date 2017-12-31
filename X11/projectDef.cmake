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
INSTALL(FILES ${CMAKE_SOURCE_DIR}/${FB_PLATFORM_NAME}/${PROJNAME}.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)
message("Will install .desktop file in ${CMAKE_INSTALL_PREFIX}/share/applications")

#install the icon
INSTALL(FILES ${CMAKE_SOURCE_DIR}/forms/icon_ellow.png DESTINATION ${CMAKE_INSTALL_PREFIX}/share/pixmaps)
message("Will install icon file in ${CMAKE_INSTALL_PREFIX}/share/pixmaps")


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

# change the following to e.g. appindicatorX if you want to
# build a binary without appindicator functionality on ubuntu
find_library(LIBAPPINDICATOR appindicator)
if (NOT LIBAPPINDICATOR)
	set (LIBAPPINDICATOR "")
ELSE()
	#set the compiler define
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DINDICATOR_ENABLED")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DINDICATOR_ENABLED")
	#add required include paths
	set(Gtk_INCLUDE_DIR "/usr/include/gtk-2.0")
	include_directories(${Gtk_INCLUDE_DIR})
	set(Glib_INCLUDE_DIR "/usr/include/glib-2.0")
	include_directories(${Glib_INCLUDE_DIR})
	set(Glibconfig_INCLUDE_DIR "/usr/lib/x86_64-linux-gnu/glib-2.0/include")
	include_directories(${Glibconfig_INCLUDE_DIR})
	set(Cairo_INCLUDE_DIR "/usr/include/cairo")
	include_directories(${Cairo_INCLUDE_DIR})
	set(Cairo_INCLUDE_DIR "/usr/include/cairo")
	include_directories(${Cairo_INCLUDE_DIR})
	set(Pango_INCLUDE_DIR "/usr/include/pango-1.0")
	include_directories(${Pango_INCLUDE_DIR})
	set(Gdkconfig_INCLUDE_DIR "/usr/lib/x86_64-linux-gnu/gtk-2.0/include")
	include_directories(${Gdkconfig_INCLUDE_DIR})
	set(Pixbuf_INCLUDE_DIR "/usr/include/gdk-pixbuf-2.0")
	include_directories(${Pixbuf_INCLUDE_DIR})
	set(Atk_INCLUDE_DIR "/usr/include/atk-1.0")
	include_directories(${Atk_INCLUDE_DIR})
	set(Appindicator_INCLUDE_DIR "/usr/include/libappindicator-0.1")
	include_directories(${Appindicator_INCLUDE_DIR})
ENDIF()


include_directories("${3d_party_ROOT}/include")

message("FIXING include directoryies")
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
# make replace twice because of cmake bug
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
    ${LIBAPPINDICATOR}
    )

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND 
${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/3d-party/lib-32/lib/libdivecomputer.so" "${CMAKE_CURRENT_BINARY_DIR}/bin/$<CONFIGURATION>/libdivecomputer.so")

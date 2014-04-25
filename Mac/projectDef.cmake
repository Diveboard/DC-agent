# Mac template platform definition CMake file
# Included from ../CMakeLists.txt

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch i386 -mmacosx-version-min=10.6")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch i386 -stdlib=libstdc++ -mmacosx-version-min=10.6")
set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} -arch i386")
set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -arch i386")
set(CMAKE_OJB_C_FLAGS "${CMAKE_OBJ_C_FLAGS} -arch i386")
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libstdc++")

# remember that the current source dir is the project root; this file is in Mac/
file (GLOB PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    Mac/[^.]*.cpp
    Mac/[^.]*.mm
    Mac/[^.]*.h
    Mac/[^.]*.cmake
    )

# use this to add preprocessor definitions
add_definitions(
    
)


SOURCE_GROUP(Mac FILES ${PLATFORM})

set (SOURCES
    ${SOURCES}
    ${PLATFORM}
    )

set(wxWidgets_ROOT "${CMAKE_SOURCE_DIR}/3d-party/wxWidgets-3.0.0/build-32")
set(wxWidgets_CONFIG_EXECUTABLE "${wxWidgets_ROOT}/wx-config")
find_package(wxWidgets COMPONENTS core base gl adv html xml xrc aui webview REQUIRED)
include("${wxWidgets_USE_FILE}")

set(3d_paryt_ROOT "${CMAKE_SOURCE_DIR}/3d-party/lib-32")

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

add_executable(${PROJNAME} MACOSX_BUNDLE ${SOURCES})

target_link_libraries(${PROJNAME}
    ${wxWidgets_LIBRARIES}
    ${LIBDIVECOMPUTER}
    ${Boost_LIBRARIES}
    ${LIBICONV}
    ${LIBCONFIG}
    curl
    )
    
add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND 
${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/3d-party/lib-32/lib/libdivecomputer.dylib" "${CMAKE_CURRENT_BINARY_DIR}/bin/$<CONFIGURATION>/dive-agent.app/Contents/MacOS/libdivecomputer.dylib")

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND 
${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/3d-party/lib-32/lib/libconfig++.dylib" "${CMAKE_CURRENT_BINARY_DIR}/bin/$<CONFIGURATION>/dive-agent.app/Contents/MacOS/libconfig++.dylib")


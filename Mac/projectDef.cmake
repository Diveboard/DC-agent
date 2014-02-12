# Mac template platform definition CMake file
# Included from ../CMakeLists.txt

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch i386")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch i386")
set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} -arch i386")
set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -arch i386")
set(CMAKE_OJB_C_FLAGS "${CMAKE_OBJ_C_FLAGS} -arch i386")

# remember that the current source dir is the project root; this file is in Mac/
file (GLOB PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    Mac/[^.]*.cpp
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
find_package(wxWidgets COMPONENTS core base gl adv html xml xrc aui REQUIRED)
include("${wxWidgets_USE_FILE}")

set(libdivecomputer_ROOT "${CMAKE_SOURCE_DIR}/3d-party/libdivecomputer-0.4.1")
find_library(LIBDIVECOMPUTER divecomputer ${libdivecomputer_ROOT}/build/src/.libs/ NO_DEFAULT_PATH)
include_directories("${libdivecomputer_ROOT}/include")

add_executable(${PROJNAME} MACOSX_BUNDLE ${SOURCES})

target_link_libraries(${PROJNAME}
    ${wxWidgets_LIBRARIES}
#    ${LIBDIVECOMPUTER}
    ${Boost_LIBRARIES}
    ${LIBICONV}
    )
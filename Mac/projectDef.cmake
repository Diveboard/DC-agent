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

set(wxWidgets_ROOT "../wxWidgets-3.0.0/build-release")
set(wxWidgets_CONFIG_EXECUTABLE "${wxWidgets_ROOT}/wx-config")
find_package(wxWidgets COMPONENTS core base gl adv html xml xrc aui REQUIRED)
include("${wxWidgets_USE_FILE}")

add_executable(${PROJNAME} MACOSX_BUNDLE ${SOURCES})
message("wx-libs=${wxWidgets_LIBRARIES}")

target_link_libraries(${PROJNAME}
    ${wxWidgets_LIBRARIES}
    )

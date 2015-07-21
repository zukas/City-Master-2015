#set(CMAKE_C_COMPILER /usr/bin/clang-3.6)
#set(CMAKE_CXX_COMPILER /usr/bin/clang++-3.6)

cmake_minimum_required(VERSION 2.8)
project(City-Master-2015 CXX)

message("Additional ubuntu repositories:\nppa:keithw/glfw3")
message("Required packages:\nlibghc-glut-dev libglfw3-dev libglm-dev libgl1-mesa-dev libglew-dev libfreetype6-dev")

include(FindPkgConfig)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -fPIC")

set(BUILD_TYPE ${CMAKE_BUILD_TYPE})
if (NOT "${BUILD_TYPE}" STREQUAL "")
string(TOUPPER ${BUILD_TYPE} BUILD_TYPE)
endif (NOT "${BUILD_TYPE}" STREQUAL "")

if ("${BUILD_TYPE}" STREQUAL "RELEASE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -fomit-frame-pointer")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -fomit-frame-pointer")
add_definitions(-DRELEASE_BUILD)
else ("${BUILD_TYPE}" STREQUAL "RELEASE")
add_definitions(-DDEBUG_BUILD)
endif ("${BUILD_TYPE}" STREQUAL "RELEASE")

add_definitions(-DGLM_FORCE_RADIANS)

set(OpenGL
    GL
    GLU
    glut
    GLEW
    glfw
    )

set(FreeType
    freetype2
    )

set(RESOURCES gl-res)
set(MEMORY gl-memory)
set(UTIL gl-util)
set(IMPORT gl-import)
set(UI gl-ui)
set(TESTS gl-test)

add_subdirectory(Resources)
add_subdirectory(Memory)
add_subdirectory(Util)
add_subdirectory(Import)
add_subdirectory(UI)
add_subdirectory(Tests)


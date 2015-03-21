cmake_minimum_required(VERSION 2.8)
project(City-Master-2015 CXX)

message("Additional ubuntu repositories:\nppa:keithw/glfw3")
message("Required packages:\nlibghc-glut-dev libglfw3-dev libglm-dev libgl1-mesa-dev libglew-dev libfreetype6-dev")

include(FindPkgConfig)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -Wextra -pedantic -fPIC")

set(BUILD_TYPE ${CMAKE_BUILD_TYPE})
if (NOT "${BUILD_TYPE}" STREQUAL "")
string(TOUPPER ${BUILD_TYPE} BUILD_TYPE)
if ("${BUILD_TYPE}" STREQUAL "RELEASE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -fomit-frame-pointer -pipe")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -fomit-frame-pointer -pipe")
endif ("${BUILD_TYPE}" STREQUAL "RELEASE")
endif (NOT "${BUILD_TYPE}" STREQUAL "")

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

set(UTIL gl-util)
set(IMPORT gl-import)
set(UI gl-ui)
set(TESTS gl-test)


add_subdirectory(Util)
add_subdirectory(Import)
add_subdirectory(UI)
add_subdirectory(Tests)


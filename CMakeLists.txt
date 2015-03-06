project(City-Master-2015)
cmake_minimum_required(VERSION 2.8)

include(FindPkgConfig)

list( APPEND CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -pedantic")

add_definitions(-DGLM_FORCE_RADIANS)

set(OpenGL
    GL
    GLU
    glut
    GLEW
    glfw
    )

set(SOIL
    SOIL
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


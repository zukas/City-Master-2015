project(${UTIL} CXX)

pkg_check_modules(FREETYPE2 ${FreeType})
include_directories(${FREETYPE2_INCLUDE_DIRS})

add_subdirectory(SOIL)

list(APPEND HEADERS
    types.h
    refcount.h
    program.h
    texture.h
    animation.h
    mesh.h
    model.h
    camera.h
    axis.h
    clock.h
    lamp.h
    colourid.h
    text.h
    mouse.h
    skybox.h
    )

list(APPEND SOURCES
    refcount.cpp
    program.cpp
    texture.cpp
    animation.cpp
    mesh.cpp
    model.cpp
    camera.cpp
    axis.cpp
    clock.cpp
    lamp.cpp
    colourid.cpp
    text.cpp
    mouse.cpp
    skybox.cpp
    )

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${OpenGL} gl-soil ${FREETYPE2_LIBRARIES})

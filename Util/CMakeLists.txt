project(${UTIL} CXX)

pkg_check_modules(FREETYPE2 ${FreeType})
include_directories(${FREETYPE2_INCLUDE_DIRS})

#add_subdirectory(SOIL)

file(GLOB SOURCE_FILES
    "*.h"
    "SOIL/*.h"
    "*.c"
    "*.cpp"
    "SOIL/*.c"
    "SOIL/*.cpp"
    )

#list(APPEND HEADERS
#    types.h
#    refcount.h
#    program.h
#    texture.h
#    animation.h
#    mesh.h
#    model.h
#    camera.h
#    axis.h
#    clock.h
#    lamp.h
#    colourid.h
#    text.h
#    mouse.h
#    keyboard.h
#    skybox.h
#    particlesystem.h
#    gldebugger.h
#    utils.h
#    shapes.h
#    SOIL/SOIL.h
#    SOIL/image_DXT.h
#    SOIL/image_helper.h
#    SOIL/stbi_DDS_aug.h
#    SOIL/stbi_DDS_aug_c.h
#    SOIL/stb_image_aug.h
#    )

#list(APPEND SOURCES
#    refcount.cpp
#    program.cpp
#    texture.cpp
#    animation.cpp
#    mesh.cpp
#    model.cpp
#    camera.cpp
#    axis.cpp
#    clock.cpp
#    lamp.cpp
#    colourid.cpp
#    text.cpp
#    mouse.cpp
#    keyboard.cpp
#    skybox.cpp
#    particlesystem.cpp
#    gldebugger.cpp
#    utils.cpp
#    shapes.cpp
#    SOIL/SOIL.cpp
#    SOIL/image_DXT.cpp
#    SOIL/image_helper.cpp
#    SOIL/stb_image_aug.cpp
#    )

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${OpenGL} ${FREETYPE2_LIBRARIES})

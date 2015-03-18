project(gl-soil C)

list(APPEND HEADERS
    SOIL.h
    image_DXT.h
    image_helper.h
    stbi_DDS_aug.h
    stbi_DDS_aug_c.h
    stb_image_aug.h
    )

list(APPEND SOURCES
    SOIL.c
    image_DXT.c
    image_helper.c
    stb_image_aug.c
    original/stb_image-1.09.c
    original/stb_image-1.16.c
    )

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${OpenGL})

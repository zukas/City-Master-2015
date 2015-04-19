project(${TESTS} CXX)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Util)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Import)

list(APPEND HEADERS
    glprogram.h
    )

list(APPEND SOURCES
    main.cpp
    glprogram.cpp
    )


list(APPEND SHADERS
    shaders/object.vert
    shaders/object.frag
    shaders/object_select.vert
    shaders/object_select.frag
    shaders/basic.vert
    shaders/basic.frag
    shaders/skybox.vert
    shaders/skybox.frag
    shaders/text.vert
    shaders/text.frag
    shaders/particles_update.vert
    shaders/particles_update.frag
    shaders/particles_update.geom
    shaders/particles_render.vert
    shaders/particles_render.geom
    shaders/particles_render.frag
    )

list(APPEND MODELS
    models/cube.dae
    models/sphere.dae
    )

list(APPEND TEXTURES
    textures/uv_map_texture.png
    textures/sphere_uv.png
    textures/failed.png
    textures/front.jpg
    textures/back.jpg
    textures/left.jpg
    textures/right.jpg
    textures/up.jpg
    textures/down.jpg
    textures/desertsky_bk.tga
    textures/desertsky_dn.tga
    textures/desertsky_ft.tga
    textures/desertsky_lf.tga
    textures/desertsky_rt.tga
    textures/desertsky_up.tga
    textures/ss_bk.tga
    textures/ss_dn.tga
    textures/ss_ft.tga
    textures/ss_lf.tga
    textures/ss_rt.tga
    textures/ss_up.tga
    textures/bluecloud_bk.jpg
    textures/bluecloud_dn.jpg
    textures/bluecloud_ft.jpg
    textures/bluecloud_lf.jpg
    textures/bluecloud_rt.jpg
    textures/bluecloud_up.jpg
    textures/fire.jpg
    textures/fire2.jpg
    textures/particle.bmp
    textures/particle.png
    )

list(APPEND FONTS
    fonts/FreeSans.ttf
    )

file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/shaders)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/shaders)
file(COPY ${SHADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shaders)

file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/models)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/models)
file(COPY ${MODELS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/models)

file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/textures)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/textures)
file(COPY ${TEXTURES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/textures)

file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/fonts)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/fonts)
file(COPY ${FONTS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/fonts)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${SHADERS} ${MODELS} ${TEXTURES} ${FONTS})
target_link_libraries(${PROJECT_NAME} ${UTIL} ${IMPORT})

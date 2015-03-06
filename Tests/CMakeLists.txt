project(${TESTS})

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
    )

list(APPEND MODELS
    models/build.fbx
    models/build.dae
    models/build.x
    models/box.fbx
    models/box.dae
    models/test.x
    models/test.dae
    models/test.x3d
    models/test.3ds
    models/test.fbx
    models/cube.dae
    )

list(APPEND TEXTURES
    textures/rock_stones_0154_01_preview.jpg
    textures/rock_stones_0154_01_preview.jpg.002.jpg
    textures/rock1.jpg
    textures/uv_map_texture.png
    textures/uv_map_texture.jpg
    textures/failed.png
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

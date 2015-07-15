project(${TESTS} CXX)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Util)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Import)

#list(APPEND SHADERS
#    shaders/object.vert
#    shaders/object.frag
#    shaders/object_select.vert
#    shaders/object_select.frag
#    shaders/basic.vert
#    shaders/basic.frag
#    shaders/skybox.vert
#    shaders/skybox.frag
#    shaders/text.vert
#    shaders/text.frag
#    shaders/particles_update.vert
#    shaders/particles_update.frag
#    shaders/particles_update.geom
#    shaders/particles_render.vert
#    shaders/particles_render.geom
#    shaders/particles_render.frag
#    )

#list(APPEND MODELS
#    models/cube.dae
#    models/sphere.dae
#    models/moving_ball.dae
#    )

#list(APPEND TEXTURES
#    textures/fire2.png
#    textures/sun.jpg
#    textures/earth.dds
#    textures/moon.dds
#    textures/mercury.jpg
#    textures/mercurymap.jpg
#    textures/venusmap.jpg
#    textures/mars.dds
#    textures/jupiter.jpg
#    textures/ganymede.dds
#    textures/io.png
#    textures/europa.dds
#    textures/callisto.dds
#    textures/saturn.png
#    textures/titan.png
#    textures/enceladus.png
#    textures/rhea.jpg
#    textures/dione.jpg
#    textures/tethys.jpg
#    textures/iapetus.jpg
#    textures/mimas.jpg
#    textures/uranus.jpg
#    textures/neptune.dds
#    )

#list(APPEND FONTS
#    fonts/FreeSans.ttf
#    )


#add_custom_target(resources ALL
#    DEPENDS ${RESOURCES}
#    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources
#    COMMAND ${CMAKE_SOURCE_DIR}/gl-res ${SHADERS} ${TEXTURES} ${CMAKE_CURRENT_SOURCE_DIR}/resources
#)

execute_process(COMMAND cmd /c ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources
                COMMAND cmd /c ${CMAKE_SOURCE_DIR}/gl-res ${SHADERS} ${TEXTURES} ${CMAKE_CURRENT_SOURCE_DIR}/resources
                )

file(GLOB RESOURCES
    "fonts/*.ttf"
    "textures/*.dds"
    "textures/*.jpg"
    "textures/*.png"
    "shaders/*.glsl"
    )

file(GLOB SOURCES
    "*.h"
    "*.c"
    "*.cpp"
    "resources/*.h"
    "resources/*.cpp"
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

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${RESOURCES})
target_link_libraries(${PROJECT_NAME} ${UTIL} ${IMPORT} )

#message("${CMAKE_MAKE_PROGRAM}")

#add_custom_command(TARGET ${PROJECT_NAME}
#                    PRE_BUILD
#                    COMMAND ${CMAKE_COMMAND} -E make_directory resources
#                    COMMAND ${CMAKE_SOURCE_DIR}/gl-res ${SHADERS} resources | ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS} -c | ${CMAKE_AR} rcs resources.a
#                    )


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

#list(APPEND TEXTURES
#    textures/callisto.dds
#    textures/dione.dds
#    textures/earth.dds
#    textures/enceladus.dds
#    textures/europa.dds
#    textures/ganymede.dds
#    textures/iapetus.dds
#    textures/io.dds
#    textures/jupiter.dds
#    textures/mars.dds
#    textures/mercury.dds
#    textures/mercurymap.dds
#    textures/mimas.dds
#    textures/moon.dds
#    textures/neptune.dds
#    textures/rhea.dds
#    textures/saturn.dds
#    textures/sun.dds
#    textures/tethys.dds
#    textures/titan.dds
#    textures/uranus.dds
#    textures/venusmap.dds
#    )

#list(APPEND FONTS
#    fonts/FreeSans.ttf
#    )

file(GLOB RES_FILES
    "shaders/*.glsl"
    "fonts/*.ttf"
    "textures/*.dds"
    "textures/*.jpg"
    "textures/*.png"
    )

#list(APPEND RESOURCE_SOURCES
#    resources/res_object_vert.h
#    resources/res_object_vert.cpp
#    resources/res_object_frag.h
#    resources/res_object_frag.cpp
#    resources/res_callisto_dds.h
#    resources/res_callisto_dds.cpp
#    resources/res_dione_dds.h
#    resources/res_dione_dds.cpp
#    resources/res_earth_dds.h
#    resources/res_earth_dds.cpp
#    resources/res_enceladus_dds.h
#    resources/res_enceladus_dds.cpp
#    resources/res_europa_dds.h
#    resources/res_europa_dds.cpp
#    resources/res_ganymede_dds.h
#    resources/res_ganymede_dds.cpp
#    resources/res_iapetus_dds.h
#    resources/res_iapetus_dds.cpp
#    resources/res_io_dds.h
#    resources/res_io_dds.cpp
#    resources/res_jupiter_dds.h
#    resources/res_jupiter_dds.cpp
#    resources/res_mars_dds.h
#    resources/res_mars_dds.cpp
#    resources/res_mercury_dds.h
#    resources/res_mercury_dds.cpp
#    resources/res_mercurymap_dds.h
#    resources/res_mercurymap_dds.cpp
#    resources/res_mimas_dds.h
#    resources/res_mimas_dds.cpp
#    resources/res_moon_dds.h
#    resources/res_moon_dds.cpp
#    resources/res_neptune_dds.h
#    resources/res_neptune_dds.cpp
#    resources/res_rhea_dds.h
#    resources/res_rhea_dds.cpp
#    resources/res_saturn_dds.h
#    resources/res_saturn_dds.cpp
#    resources/res_sun_dds.h
#    resources/res_sun_dds.cpp
#    resources/res_tethys_dds.h
#    resources/res_tethys_dds.cpp
#    resources/res_titan_dds.h
#    resources/res_titan_dds.cpp
#    resources/res_uranus_dds.h
#    resources/res_uranus_dds.cpp
#    resources/res_venusmap_dds.h
#    resources/res_venusmap_dds.cpp
#    )


#list(APPEND RES_LIST
#    ${SHADERS}
#    ${TEXTURES}
#    ${FONTS}
#    )

execute_process(
    COMMAND ${CMAKE_COMMAND} -E make_directory resources
    COMMAND ${CMAKE_SOURCE_DIR}/gl-res ${RES_FILES} resources
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE MY_PATH
    )

#message("${MY_PATH}")

file(GLOB SRC_FILES
    "*.c"
    "*.cpp"
    "*.h"
    "*.hpp"
    "resources/*.h"
    "resources/*.cpp"
    )

#list(APPEND SRC_LIST
#    main.cpp
#    glprogram.h
#    glprogram.cpp
#    saturnrings.h
#    saturnrings.cpp
#    ${RESOURCE_SOURCES}
#    )

add_custom_target(resources ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory resources
    COMMAND ${CMAKE_SOURCE_DIR}/gl-res ${RES_FILES} resources
    DEPENDS ${RESOURCES} ${RES_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${RES_FILES} )
target_link_libraries(${PROJECT_NAME} ${UTIL} ${IMPORT})
add_dependencies(${PROJECT_NAME} resources)



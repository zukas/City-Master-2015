project(${TESTS} CXX)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

file(GLOB RES_FILES
    "shaders/*.glsl"
    "fonts/*.ttf"
    "textures/*.dds"
    "textures/*.jpg"
    "textures/*.png"
    )

execute_process(
    COMMAND ${CMAKE_COMMAND} -E make_directory resources
    COMMAND ${CMAKE_SOURCE_DIR}/gl-res ${RES_FILES} rc_pack.crp resources
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE __OUT_RES
    )

file(GLOB SRC_FILES
    "*.c"
    "*.cpp"
    "*.h"
    "*.hpp"
    "resources/*.h"
    "resources/*.cpp"
    )

add_custom_target(resources ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory resources
    COMMAND ${CMAKE_SOURCE_DIR}/gl-res ${RES_FILES} rc_pack.crp resources
    DEPENDS ${RESOURCES} ${RES_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${RES_FILES} )
target_link_libraries(${PROJECT_NAME} ${MEMORY} ${UTIL} ${IMPORT})
add_dependencies(${PROJECT_NAME} resources)



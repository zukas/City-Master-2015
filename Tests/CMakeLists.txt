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
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/rc_pack.crp
        ${CMAKE_CURRENT_BINARY_DIR}/rc_pack.crp
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
    "${CMAKE_SOURCE_DIR}/Common/*.h"
    )

add_custom_target(resources ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory resources
    COMMAND ${CMAKE_SOURCE_DIR}/gl-res ${RES_FILES} rc_pack.crp resources
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/rc_pack.crp
        ${CMAKE_CURRENT_BINARY_DIR}/rc_pack.crp
    DEPENDS ${RESOURCES} ${RES_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${RES_FILES} )
target_link_libraries(${PROJECT_NAME} ${MEMORY} ${UTIL} ${UI})
add_dependencies(${PROJECT_NAME} resources)



project(${IMPORT} CXX)
add_subdirectory(Assimp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Assimp/include)
include_directories(${CMAKE_SOURCE_DIR}/Util)

list(APPEND HEADERS
    loader.h
    )

list(APPEND SOURCES
    loader.cpp
    )

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} assimp ${OpenGL} ${SOIL} ${UTIL})

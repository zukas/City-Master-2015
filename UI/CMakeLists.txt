project(${UI} CXX)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

list(APPEND HEADERS
    box2d.h
    )

list(APPEND SOURCES
    box2d.cpp
    )

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${MEMORY} ${UTIL} ${IMPORT})

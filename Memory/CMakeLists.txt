project(${MEMORY} CXX)

file(GLOB SOURCES
    "*.h"
    "*.cpp"
    "${CMAKE_SOURCE_DIR}/Common/*.h"
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

add_library(${PROJECT_NAME} SHARED ${SOURCES})

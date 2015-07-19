project(${MEMORY} CXX)

file(GLOB SOURCES
    "*.h"
    "*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

project(${UTIL} CXX)

pkg_check_modules(FREETYPE2 ${FreeType})
include_directories(${FREETYPE2_INCLUDE_DIRS})

file(GLOB SOURCE_FILES
    "*.h"
    "SOIL/*.h"
    "*.c"
    "*.cpp"
    "SOIL/*.c"
    "SOIL/*.cpp"
    "${CMAKE_SOURCE_DIR}/Common/*.h"
    )

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${OpenGL} ${FREETYPE2_LIBRARIES})

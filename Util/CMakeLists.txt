project(${UTIL} CXX)

pkg_check_modules(FREETYPE2 ${FreeType})
include_directories(${FREETYPE2_INCLUDE_DIRS})

file(GLOB SOIL_FILES
    "SOIL/*.h"
    "SOIL/*.c"
    "SOIL/*.cpp"
    )

#SET_SOURCE_FILES_PROPERTIES(${SOIL_FILES} PROPERTIES LANGUAGE C )

file(GLOB SOURCE_FILES
    "*.h"
    "*.cpp"
    "${CMAKE_SOURCE_DIR}/Common/*.h"
    )

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${SOIL_FILES})
target_link_libraries(${PROJECT_NAME} ${OpenGL} ${FREETYPE2_LIBRARIES})

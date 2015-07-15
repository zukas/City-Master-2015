project(${RESOURCES} CXX)

#list(APPEND HEADERS
#    resources.h
#    hash.h
#    )

#list(APPEND SOURCES
#    main.cpp
#    resources.cpp
#    )

#list(APPEND OTHER
#    sample.res
#    )

file(GLOB SOURCES
    "*.h"
    "*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

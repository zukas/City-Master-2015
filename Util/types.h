#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>

typedef unsigned GLID;
typedef int GLSID;

constexpr float PI { 3.14 };

struct Error
{
        std::string message;
        enum
        {
            NONE,

            FAILED_INIT_GLFW,
            FAILED_INIT_GLEW,

            FAILED_COMPILE_FRAGMENT_SHADER,
            FAILED_COMPILE_VERTEX_SHADER,
            FAILED_LINK_PROGRAM,

            NOT_SUPPORTED_GL3_3,

            FILE_NOT_FOUND
        } code { NONE };
};




#endif // TYPES_H

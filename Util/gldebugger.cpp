#include "gldebugger.h"
#include <GL/glew.h>
#include <GL/gl.h>

glDebugger::glDebugger()
{
}

glDebugger &glDebugger::get()
{
    static glDebugger debug;
    return debug;
}

void glDebugger::clear()
{
    glGetError();
}

void glDebugger::inspect(const char *file, const char *function, int line)
{
    auto error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("%s - %s(%d) Error: %d\n", file, function, line, error);
    }
}

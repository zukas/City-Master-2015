#ifndef GLDEBUGGER_H
#define GLDEBUGGER_H

#include <functional>

class glDebugger
{

private:


private:
    glDebugger();
    glDebugger &get();


public:
    static void clear();
    static void inspect(const char *file, const char *function, int line);
};

#define GLCLEAR() glDebugger::clear();
#define GLCHECK() glDebugger::inspect(__FILE__, __func__, __LINE__)


#endif //GLDEBUGGER_H

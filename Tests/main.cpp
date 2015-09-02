#include "glprogram.h"
#include "Util/profiler.h"

int main() {
    glProgram p;
    p.exec();

    print_profiler_results();
    return 0;
}

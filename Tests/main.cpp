#include "glprogram.h"
#include "Util/mesh3d.h"
#include "Util/mesh.h"
#include "Util/model.h"
#include "Util/profiler.h"

int main()
{
    printf("Model: %lu\nMesh: %lu\nMesh3D: %lu\n", sizeof(Model), sizeof(Mesh), sizeof(Mesh3D));

	glProgram p;
	p.exec();

    print_profiler_results();
    return 0;
}

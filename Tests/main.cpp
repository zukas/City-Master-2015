#include "glprogram.h"
#include "Util/mesh3d.h"
#include "Util/mesh.h"
#include "Util/model.h"
#include "Util/profiler.h"

int main()
{
    printf("Model: %lu\nMesh: %lu\nMesh3D: %lu\n", sizeof(Model), sizeof(Mesh), sizeof(Mesh3D));
	printf("r: %u, g: %u, b: %u, a: %u\n", 255, 255 << 8, 255 << 16, 255 << 24);

	glProgram p;
	p.exec();

    print_profiler_results();
    return 0;
}

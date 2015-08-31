#ifndef SHAPES_H
#define SHAPES_H

#include "types.h"

namespace sphare
{
	shape create(float radius, int slices, int stacks);
    void create(uv_vertex *vertexes, uint32_t indexes, float radius, uint32_t slices, uint32_t stacks);
}

namespace cube
{

}

#endif // SHAPES_H

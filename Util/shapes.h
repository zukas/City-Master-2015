#ifndef SHAPES_H
#define SHAPES_H

#include "types.h"

namespace sphare {
shape create(float radius, int slices, int stacks);
void create(uv_vertex *vertexes, uint32_t *indexes, float radius,
            uint32_t units);
constexpr uint32_t vertex_size(int32_t units) { return (units + 1) * (units + 1); }
constexpr uint32_t index_size(int32_t units) {
	return (units * units + units) * 6;
}
}

namespace cube {}

#endif // SHAPES_H

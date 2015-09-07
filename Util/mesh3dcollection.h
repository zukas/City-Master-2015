#ifndef MESH3DCOLLECTION_H
#define MESH3DCOLLECTION_H

#include "types.h"

class Mesh3DCollection {
  public:
    Mesh3DCollection() = delete;

	static uint32_t create(const uv_vertex *vertexes, uint32_t vertexe_size,
						   const uint32_t *indexes, uint32_t index_size);

	static uint32_t create(const glm::vec3 *vertexes, uint32_t vertexe_size,
						   const uint32_t *indexes, uint32_t index_size);

	static void destroy(uint32_t mesh_id);

	static void render_geometry(uint32_t mesh_id, uint32_t elem_count);
};

#endif // MESH3DCOLLECTION_H

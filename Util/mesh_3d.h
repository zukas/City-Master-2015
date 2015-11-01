#ifndef MESH_3D_H
#define MESH_3D_H

#include "types.h"

namespace mesh_3d {

uint32_t create(const obj_3d_vertex *vertexes, uint32_t vertexe_size,
                const uint32_t *indexes, uint32_t index_size);

uint32_t create_instanced(const obj_3d_vertex *vertexes, uint32_t vertexe_size,
                          const uint32_t *indexes, uint32_t index_size,
                          uint32_t instance_count);

uint32_t create(const uv_3d_vertex *vertexes, uint32_t vertexe_size,
                const uint32_t *indexes, uint32_t index_size);

uint32_t create(const glm::vec3 *vertexes, uint32_t vertexe_size,
                const uint32_t *indexes, uint32_t index_size);

void destroy(uint32_t mesh_id);

void render_geometry(uint32_t mesh_id, uint32_t elem_count);

void render_geometry_instanced(uint32_t mesh_id, uint32_t elem_count,
                               glm::mat4 *instance_models, uint32_t model_count);
}

#endif // MESH_3D_H

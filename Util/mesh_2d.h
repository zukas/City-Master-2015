#ifndef MESH_2D_H
#define MESH_2D_H

#include "types.h"

namespace mesh_2d {
    uint32_t create(const uv_2d_vertex *vertexes, uint32_t vertex_size);

    void destroy(uint32_t mesh_id);

    void render_geometry(uint32_t mesh_id, uint32_t vertex_size);
}

#endif // MESH_2D_H

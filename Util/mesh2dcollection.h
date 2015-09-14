#ifndef MESH2DCOLLECTION_H
#define MESH2DCOLLECTION_H

#include "types.h"

class Mesh2dCollection {
  public:
    Mesh2dCollection() = delete;

    static uint32_t create(const uv_2d_vertex *vertexes, uint32_t vertex_size);

    static void destroy(uint32_t mesh_id);

    static void render_geometry(uint32_t mesh_id, uint32_t vertex_size);
};

#endif // MESH2DCOLLECTION_H

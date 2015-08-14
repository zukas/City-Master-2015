#ifndef MESH3DCOLLECTION_H
#define MESH3DCOLLECTION_H

#include "types.h"

#ifndef MAX_MESH_3D_SIZE
#define MAX_MESH_3D_SIZE 64
#endif


class Mesh3DCollection {
  private:
    static uint32_t _elem_count[MAX_MESH_3D_SIZE];
    static uint32_t _size;

  public:
    Mesh3DCollection() = delete;
    static void init(uint32_t size);
    static void destroy();
    static uint32_t size();

    static void create(uint32_t index, const uv_vertex *vertexes,
                       uint32_t vertexe_size, const uint32_t *indexes,
                       uint32_t index_size);

    static void render_geometry(uint32_t index);
};

#endif // MESH3DCOLLECTION_H

#ifndef MESH3D_H
#define MESH3D_H

#include "types.h"

#define MESH3D_TEXTURE_COUNT 2

class Mesh3D {

  private:
	//Object size 32 bytes on 64 bit machine
    GLID m_vertex_array{0};
    uint32_t m_count{0};
    GLID m_textures[MESH3D_TEXTURE_COUNT]{0};
    uint32_t m_size;

  public:
    Mesh3D();
    Mesh3D(const Mesh3D &) = delete;
    Mesh3D(Mesh3D &&other);
    Mesh3D(const uv_vertex *vertexes, const GLID *indexes, uint32_t size);
    ~Mesh3D();
    void add_texture(GLID texture_id);
    void render() const;
    void render_geometry() const;

    Mesh3D &operator=(const Mesh3D &) = delete;
    Mesh3D &operator=(Mesh3D &&other);
};

#endif // MESH3D_H

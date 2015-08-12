#ifndef MESH3D_H
#define MESH3D_H

#include "types.h"
#include "texture2d.h"

#define MESH3D_TEXTURE_COUNT 2

class Mesh3D {

  private:
	// Object size 32 bytes on 64 bit machine
    GLID m_vertex_array{0};
	Texture2DArray<MESH3D_TEXTURE_COUNT > m_textures;
	uint32_t m_elem_size{0};

  public:
	Mesh3D() = default;
    Mesh3D(const Mesh3D &) = delete;
	Mesh3D(Mesh3D &&other) = default;
	Mesh3D(const uv_vertex *vertexes, uint32_t vertexe_size,
		   const uint32_t *indexes, uint32_t index_size);
    ~Mesh3D();
	Texture2D& texture(uint32_t index);
    void render() const;
    void render_geometry() const;

	Mesh3D &&move();

    Mesh3D &operator=(const Mesh3D &) = delete;
	Mesh3D &operator=(Mesh3D &&other) = default;
};

#endif // MESH3D_H

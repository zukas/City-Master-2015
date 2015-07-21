#ifndef MESH3D_H
#define MESH3D_H

#include "types.h"

class Mesh3D {

	typedef void (*render_func)(size_t);

  private:
	GLID m_vertex_array{0};
	const size_t m_size;
	GLID m_textures[2]{0};
	const render_func m_render_func;

  public:
	Mesh3D();
	Mesh3D(const Mesh3D &) = delete;
	Mesh3D(Mesh3D &&other);
	Mesh3D(const uv_vertex *vertexes, size_t size);
	Mesh3D(const uv_vertex *vertexes, const GLID *indexes, size_t size);
	~Mesh3D();
	void set_texture(GLID index, GLID texture_id);
	void render() const;
	void render_geometry() const;
};

#endif // MESH3D_H

#include "mesh3d.h"
#include "texturecollection2d.h"
#include <GL/glew.h>


void bind_vertex_data(const uv_vertex *vertexes, size_t size) {
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(uv_vertex), vertexes,
				 GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,                 // attribute
						  3,                 // size
						  GL_FLOAT,          // type
						  GL_FALSE,          // normalized?
						  sizeof(uv_vertex), // stride
						  0                  // array buffer offset
						  );

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                                     // attribute
		3,                                     // size
		GL_FLOAT,                              // type
		GL_FALSE,                              // normalized?
		sizeof(uv_vertex),                     // stride
		reinterpret_cast<void *>(sizeof(vec3)) // array buffer offset
		);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,                                         // attribute
		2,                                         // size
		GL_FLOAT,                                  // type
		GL_FALSE,                                  // normalized?
		sizeof(uv_vertex),                         // stride
		reinterpret_cast<void *>(sizeof(vec3) * 2) // array buffer offset
		);
}

void render_with_index(size_t size) {
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr);
}

void render_without_index(size_t size) { glDrawArrays(GL_TRIANGLES, 0, size); }

Mesh3D::Mesh3D() : m_size(0), m_render_func(nullptr) {}

Mesh3D::Mesh3D(Mesh3D &&other)
	: m_vertex_array(other.m_vertex_array), m_size(other.m_size),
	  m_render_func(other.m_render_func) {
	for (int i = 0; i < 2; ++i) {
		m_textures[i] = other.m_textures[i];
		other.m_textures[i] = 0;
	}
	other.m_vertex_array = 0;
}

Mesh3D::Mesh3D(const uv_vertex *vertexes, size_t size)
	: m_size(size), m_render_func(render_without_index) {
	glGenVertexArrays(1, &m_vertex_array);
	glBindVertexArray(m_vertex_array);

	GLID buffer[1];
	glGenBuffers(1, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	bind_vertex_data(vertexes, size);

	glBindVertexArray(0);
}

Mesh3D::Mesh3D(const uv_vertex *vertexes, const GLID *indexes, size_t size)
	: m_size(size), m_render_func(render_with_index) {
	glGenVertexArrays(1, &m_vertex_array);
	glBindVertexArray(m_vertex_array);

	GLID buffer[2];
	glGenBuffers(2, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	bind_vertex_data(vertexes, size);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLID), indexes,
				 GL_STATIC_DRAW);
	glBindVertexArray(0);
}

Mesh3D::~Mesh3D() {
	glDeleteVertexArrays(1, &m_vertex_array);
	m_vertex_array = 0;
}

void Mesh3D::set_texture(GLID index, GLID texture_id)
{
	ASSERT(index < 2);
	m_textures[index] = texture_id;
}

void Mesh3D::render() const {
	glBindVertexArray(m_vertex_array);


	//Bind the samplers here

	//Binding set textures
	for(size_t i = 0; i < 2; ++i)
	{
		TextureCollection2D::bind(m_textures[i], i);
	}

	ASSERT(m_render_func != nullptr);
	m_render_func(m_size);
	glBindVertexArray(0);
}

void Mesh3D::render_geometry() const {
	glBindVertexArray(m_vertex_array);
	ASSERT(m_render_func != nullptr);
	m_render_func(m_size);
	glBindVertexArray(0);
}

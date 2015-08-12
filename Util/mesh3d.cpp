#include "mesh3d.h"
#include "uniforms.h"
#include "profiler.h"
#include <GL/glew.h>

void bind_vertex_data(const uv_vertex *vertexes, uint32_t size) {
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

Mesh3D::Mesh3D(const uv_vertex *vertexes, uint32_t vertexe_size, const uint32_t *indexes, uint32_t index_size)
    : m_elem_size(index_size) {
    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);

    GLID buffer[2];
	glGenBuffers(2, (GLuint*)buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    bind_vertex_data(vertexes, vertexe_size);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size * sizeof(GLID), indexes,
                 GL_STATIC_DRAW);
    glBindVertexArray(0);
}

Mesh3D::~Mesh3D() {
    glDeleteVertexArrays(1, &m_vertex_array);
	m_vertex_array = 0;
}

Texture2D &Mesh3D::texture(uint32_t index)
{
	return m_textures[index];
}

void Mesh3D::render() const {
    PROF;
	const GLuint vertex_aray_ = m_vertex_array;
    const uint32_t size_ = m_elem_size;

    glBindVertexArray(vertex_aray_);

	for (uint8_t i = 0; i < m_textures.size(); ++i) {
		m_textures[i].bind(i);
        glUniform1i(glsl_sampler_int[i], i);
    }
	glUniform1i(glsl_texture_count_int,  m_textures.size());

    glDrawElements(GL_TRIANGLES, size_, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}

void Mesh3D::render_geometry() const {

	const GLuint vertex_aray_ = m_vertex_array;
    const uint32_t size_ = m_elem_size;

    glBindVertexArray(vertex_aray_);
    glDrawElements(GL_TRIANGLES, size_, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

Mesh3D &&Mesh3D::move()
{
	return ::move(*this);
}

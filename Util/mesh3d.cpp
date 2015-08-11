#include "mesh3d.h"
#include "texturecollection2d.h"
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


Mesh3D::Mesh3D() : m_size(0) {}

Mesh3D::Mesh3D(Mesh3D &&other)
    : m_vertex_array(other.m_vertex_array), m_size(other.m_size) {
    for (uint32_t i = 0; i < MESH3D_TEXTURE_COUNT; ++i) {
        m_textures[i] = other.m_textures[i];
        other.m_textures[i] = 0;
    }
    other.m_vertex_array = 0;
}

Mesh3D::Mesh3D(const uv_vertex *vertexes, const GLID *indexes, uint32_t size)
    : m_size(size) {
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

void Mesh3D::add_texture(GLID texture_id) {
    ASSERT(texture_id != 0);
    ASSERT(m_count < MESH3D_TEXTURE_COUNT);
    m_textures[m_count++] = texture_id;
}

void Mesh3D::render() const {
    PROF;
    const GLID vertex_aray_ = m_vertex_array;
    const uint32_t count_ = m_count;
    const GLID *textures_ = m_textures;
    const uint32_t size_ = m_size;

    glBindVertexArray(vertex_aray_);

    for (size_t i = 0; i < count_; ++i) {
        TextureCollection2D::bind(textures_[i], i);
        glUniform1i(glsl_sampler_int[i], i);
    }
    glUniform1i(glsl_texture_count_int, count_);

    glDrawElements(GL_TRIANGLES, size_, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}

void Mesh3D::render_geometry() const {

    const GLID vertex_aray_ = m_vertex_array;
    const uint32_t size_ = m_size;

    glBindVertexArray(vertex_aray_);
    glDrawElements(GL_TRIANGLES, size_, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

Mesh3D &Mesh3D::operator=(Mesh3D &&other) {
    m_vertex_array = other.m_vertex_array;
    m_size = other.m_size;
    for (uint32_t i = 0; i < MESH3D_TEXTURE_COUNT; ++i) {
        m_textures[i] = other.m_textures[i];
        other.m_textures[i] = 0;
    }
    m_vertex_array = 0;
    return *this;
}

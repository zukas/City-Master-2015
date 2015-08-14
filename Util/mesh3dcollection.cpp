#include "mesh3dcollection.h"
#include "profiler.h"

#include <GL/glew.h>
#include <string.h>

uint32_t Mesh3DCollection::_elem_count[MAX_MESH_3D_SIZE] { 0 };
uint32_t Mesh3DCollection::_size{0};

void Mesh3DCollection::init(uint32_t size) {
    ASSERT(_elem_count == nullptr);
    ASSERT(size > 0);
    ASSERT(size < MAX_MESH_3D_COUNT);
    _size = size;
    memset(_elem_count, 0, sizeof(_elem_count));

    uint32_t vertex_array_[MAX_MESH_3D_SIZE];
    glGenVertexArrays(size, vertex_array_);
}

void Mesh3DCollection::destroy() {
    ASSERT(_size > 0);
    const uint32_t size_ = _size;
    uint32_t vertex_array_[MAX_MESH_3D_SIZE];
    for (uint32_t i = 0; i < size_; ++i) {
        vertex_array_[i] = i + 1;
    }
    glDeleteVertexArrays(size_, vertex_array_);
}

uint32_t Mesh3DCollection::size() { return _size; }

void Mesh3DCollection::create(uint32_t index, const uv_vertex *vertexes,
                              uint32_t vertexe_size, const uint32_t *indexes,
                              uint32_t index_size) {

    ASSERT(index >= 0);
    ASSERT(index < _size);
    ASSERT(_elem_count[index] == 0);

    const uint32_t vertex_array_ = index + 1;
    uint32_t buffer_[2];

    glBindVertexArray(vertex_array_);
    glGenBuffers(2, buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_[0]);

    glBufferData(GL_ARRAY_BUFFER, vertexe_size * sizeof(uv_vertex), vertexes,
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
        1,                                          // attribute
        3,                                          // size
        GL_FLOAT,                                   // type
        GL_FALSE,                                   // normalized?
        sizeof(uv_vertex),                          // stride
        reinterpret_cast<void *>(sizeof(float) * 3) // array buffer offset
        );

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,                                          // attribute
        2,                                          // size
        GL_FLOAT,                                   // type
        GL_FALSE,                                   // normalized?
        sizeof(uv_vertex),                          // stride
        reinterpret_cast<void *>(sizeof(float) * 6) // array buffer offset
        );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[1]);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size * sizeof(GLID), indexes,
                 GL_STATIC_DRAW);
    glBindVertexArray(0);

    _elem_count[index] = index_size;
}

void Mesh3DCollection::render_geometry(uint32_t index) {
    PROF;
    ASSERT(index >= 0);
    ASSERT(index < _size);

    const uint32_t elem_count_ = _elem_count[index];

    ASSERT(elem_count_ == 0);

    glBindVertexArray(index + 1);
    glDrawElements(GL_TRIANGLES, elem_count_, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

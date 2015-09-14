#include "mesh2dcollection.h"
#include "profiler.h"

#include <GL/glew.h>
#include <string.h>

uint32_t Mesh2dCollection::create(const uv_2d_vertex *vertexes,
                                  uint32_t vertex_size) {

    uint32_t vertex_array_;
    uint32_t buffer_[1];

    glGenVertexArrays(1, &vertex_array_);

    glBindVertexArray(vertex_array_);
    glGenBuffers(1, buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_[0]);

    glBufferData(GL_ARRAY_BUFFER, vertex_size * sizeof(uv_2d_vertex), vertexes,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,                 // attribute
                          2,                 // size
                          GL_FLOAT,          // type
                          GL_FALSE,          // normalized?
                          sizeof(uv_2d_vertex), // stride
                          0                  // array buffer offset
                          );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,                                          // attribute
        2,                                          // size
        GL_FLOAT,                                   // type
        GL_FALSE,                                   // normalized?
        sizeof(uv_2d_vertex),                          // stride
        reinterpret_cast<void *>(sizeof(float) * 2) // array buffer offset
        );

    glBindVertexArray(0);
    return vertex_array_;
}

void Mesh2dCollection::destroy(uint32_t mesh_id) {
    glDeleteVertexArrays(1, &mesh_id);
}

void Mesh2dCollection::render_geometry(uint32_t mesh_id, uint32_t vertex_size) {
    PROF("Render 2d mesh geometry");
    ASSERT(vertex_size > 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(mesh_id);
    glBindVertexArray(0);
}


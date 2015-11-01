#include "mesh_3d.h"
#include "buffer_store.h"
#include "profiler.h"

#include <GL/glew.h>
#include <string.h>

uint32_t mesh_3d::create(const obj_3d_vertex *vertexes, uint32_t vertexe_size,
                         const uint32_t *indexes, uint32_t index_size) {

    uint32_t vertex_array_;

    glGenVertexArrays(1, &vertex_array_);
    uint32_t *buffer_ = buffer_store::ceate(vertex_array_, 2);

    glBindVertexArray(vertex_array_);
    glGenBuffers(2, buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_[0]);

    glBufferData(GL_ARRAY_BUFFER, vertexe_size * sizeof(obj_3d_vertex),
                 vertexes, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,                     // attribute
                          3,                     // size
                          GL_FLOAT,              // type
                          GL_FALSE,              // normalized?
                          sizeof(obj_3d_vertex), // stride
                          0                      // array buffer offset
                          );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,                                          // attribute
        3,                                          // size
        GL_FLOAT,                                   // type
        GL_TRUE,                                    // normalized?
        sizeof(obj_3d_vertex),                      // stride
        reinterpret_cast<void *>(sizeof(float) * 3) // array buffer offset
        );

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,                                          // attribute
        2,                                          // size
        GL_FLOAT,                                   // type
        GL_FALSE,                                   // normalized?
        sizeof(obj_3d_vertex),                      // stride
        reinterpret_cast<void *>(sizeof(float) * 6) // array buffer offset
        );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[1]);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size * sizeof(uint32_t),
                 indexes, GL_STATIC_DRAW);
    glBindVertexArray(0);
    return vertex_array_;
}

uint32_t mesh_3d::create_instanced(const obj_3d_vertex *vertexes,
                                   uint32_t vertexe_size,
                                   const uint32_t *indexes, uint32_t index_size,
                                   uint32_t instance_count) {

    uint32_t vertex_array_;

    glGenVertexArrays(1, &vertex_array_);
    uint32_t *buffer_ = buffer_store::get(vertex_array_, 3);

    glBindVertexArray(vertex_array_);
    glGenBuffers(3, buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_[0]);

    glBufferData(GL_ARRAY_BUFFER, vertexe_size * sizeof(obj_3d_vertex),
                 vertexes, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,                     // attribute
                          3,                     // size
                          GL_FLOAT,              // type
                          GL_FALSE,              // normalized?
                          sizeof(obj_3d_vertex), // stride
                          0                      // array buffer offset
                          );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,                                          // attribute
        3,                                          // size
        GL_FLOAT,                                   // type
        GL_TRUE,                                    // normalized?
        sizeof(obj_3d_vertex),                      // stride
        reinterpret_cast<void *>(sizeof(float) * 3) // array buffer offset
        );

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,                                          // attribute
        2,                                          // size
        GL_FLOAT,                                   // type
        GL_FALSE,                                   // normalized?
        sizeof(obj_3d_vertex),                      // stride
        reinterpret_cast<void *>(sizeof(float) * 6) // array buffer offset
        );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[1]);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size * sizeof(uint32_t),
                 indexes, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffer_[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instance_count, nullptr,
                 GL_STREAM_DRAW);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (GLvoid *)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (GLvoid *)sizeof(glm::mat4));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (GLvoid *)(sizeof(glm::mat4) * 2));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (GLvoid *)(sizeof(glm::mat4) * 3));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
    return vertex_array_;
}

uint32_t mesh_3d::create(const uv_3d_vertex *vertexes, uint32_t vertexe_size,
                         const uint32_t *indexes, uint32_t index_size) {
    uint32_t vertex_array_;

    glGenVertexArrays(1, &vertex_array_);
    uint32_t *buffer_ = buffer_store::get(vertex_array_, 2);

    glBindVertexArray(vertex_array_);
    glGenBuffers(2, buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_[0]);

    glBufferData(GL_ARRAY_BUFFER, vertexe_size * sizeof(uv_3d_vertex), vertexes,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,                    // attribute
                          3,                    // size
                          GL_FLOAT,             // type
                          GL_FALSE,             // normalized?
                          sizeof(uv_3d_vertex), // stride
                          0                     // array buffer offset
                          );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,                                          // attribute
        2,                                          // size
        GL_FLOAT,                                   // type
        GL_FALSE,                                   // normalized?
        sizeof(uv_3d_vertex),                       // stride
        reinterpret_cast<void *>(sizeof(float) * 3) // array buffer offset
        );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[1]);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size * sizeof(uint32_t),
                 indexes, GL_STATIC_DRAW);
    glBindVertexArray(0);
    return vertex_array_;
}

uint32_t mesh_3d::create(const glm::vec3 *vertexes, uint32_t vertexe_size,
                         const uint32_t *indexes, uint32_t index_size) {
    uint32_t vertex_array_;

    glGenVertexArrays(1, &vertex_array_);
    uint32_t *buffer_ = buffer_store::get(vertex_array_, 2);

    glBindVertexArray(vertex_array_);
    glGenBuffers(2, buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_[0]);

    glBufferData(GL_ARRAY_BUFFER, vertexe_size * sizeof(glm::vec3), vertexes,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,                 // attribute
                          3,                 // size
                          GL_FLOAT,          // type
                          GL_FALSE,          // normalized?
                          sizeof(glm::vec3), // stride
                          nullptr            // array buffer offset
                          );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[1]);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size * sizeof(uint32_t),
                 indexes, GL_STATIC_DRAW);
    glBindVertexArray(0);
    return vertex_array_;
}

void mesh_3d::destroy(uint32_t mesh_id) { glDeleteVertexArrays(1, &mesh_id); }

void mesh_3d::render_geometry(uint32_t mesh_id, uint32_t elem_count) {
    PROF("Render 3D mesh geometry");
    ASSERT(elem_count > 0);
    glBindVertexArray(mesh_id);
    glDrawElements(GL_TRIANGLES, elem_count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void mesh_3d::render_geometry_instanced(uint32_t mesh_id, uint32_t elem_count,
                                        glm::mat4 *instance_models,
                                        uint32_t model_count) {

    PROF("Render 3D mesh geometry instanced");
    ASSERT(elem_count > 0);
    glBindVertexArray(mesh_id);
    glDrawElementsInstanced(GL_TRIANGLES, elem_count, GL_UNSIGNED_INT, nullptr, model_count);
    glBindVertexArray(0);
}

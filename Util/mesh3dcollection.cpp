#include "mesh3dcollection.h"
#include "profiler.h"

#include <GL/glew.h>
#include <string.h>

uint32_t Mesh3DCollection::create(const uv_vertex *vertexes,
								  uint32_t vertexe_size,
								  const uint32_t *indexes,
								  uint32_t index_size) {

	uint32_t vertex_array_;
	uint32_t buffer_[2];

	glGenVertexArrays(1, &vertex_array_);

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
		GL_TRUE,                                   // normalized?
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

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size * sizeof(uint32_t),
				 indexes, GL_STATIC_DRAW);
    glBindVertexArray(0);
	return vertex_array_;
}

uint32_t Mesh3DCollection::create(const glm::vec3 *vertexes,
								  uint32_t vertexe_size,
								  const uint32_t *indexes,
								  uint32_t index_size) {
	uint32_t vertex_array_;
	uint32_t buffer_[2];

	glGenVertexArrays(1, &vertex_array_);

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

void Mesh3DCollection::destroy(uint32_t mesh_id) {
	glDeleteVertexArrays(1, &mesh_id);
}

void Mesh3DCollection::render_geometry(uint32_t mesh_id, uint32_t elem_count) {
	PROF("Render 3D mesh geometry");
	ASSERT(elem_count > 0);
	glBindVertexArray(mesh_id);
	glDrawElements(GL_TRIANGLES, elem_count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

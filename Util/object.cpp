#include "object.h"
#include <GL/glew.h>
#include <GL/gl.h>

Object::Object(std::vector<glm::vec3> &&vertex, std::vector<glm::vec3 > &&normals, std::vector<glm::vec2 > &&uvs) :
    m_vertexes(vertex),
    m_normals(normals),
    m_uvs(uvs)
{
    glGenVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);
    glGenBuffers(bufferCount, m_buffers);
    glBindVertexArray(0);
}

void Object::render()
{

}

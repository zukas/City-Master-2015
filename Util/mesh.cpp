#include "mesh.h"
#include "clock.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

void Mesh::bindData()
{
    glGenVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);

    int bufferCount = m_type == UV ? 3 : 2;
    GLuint *buffers = new GLuint[bufferCount];
    glGenBuffers(bufferCount, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, m_vertexes.size() * sizeof(glm::vec3), &m_vertexes[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
                0,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                0           // array buffer offset
                );

    if(m_type == UV)
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
                    1,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    0            // array buffer offset
                    );

        glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
        glBufferData(GL_ARRAY_BUFFER, m_uvs.size() * sizeof(glm::vec2), &m_uvs[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(
                    2,                  // attribute
                    2,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    0            // array buffer offset
                    );
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glBufferData(GL_ARRAY_BUFFER, m_colours.size() * sizeof(glm::vec3), &m_colours[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
                    1,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    0            // array buffer offset
                    );
    }

    delete [] buffers;
    glBindVertexArray(0);
}

Mesh::Mesh()
{

}

Mesh::Mesh(Mesh &&other) :
    modelMatrix(std::move(other.modelMatrix)),
    m_vertexArray(other.m_vertexArray),
    m_vertexes(std::move(other.m_vertexes)),
    m_normals(std::move(other.m_normals)),
    m_colours(std::move(other.m_colours)),
    m_uvs(std::move(other.m_uvs)),
    m_textures(std::move(other.m_textures)),
    m_animations(std::move(other.m_animations)),
    m_animIndex(other.m_animIndex),
    m_type(other.m_type),
    m_shape(other.m_shape)
{

    other.m_vertexArray = 0;
    other.m_animIndex = -1;
}

Mesh::Mesh(std::vector<glm::vec3> vertex, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvs) :
    modelMatrix(1.f),
    m_vertexes(std::move(vertex)),
    m_normals(std::move(normals)),
    m_uvs(std::move(uvs)),
    m_type(UV),
    m_shape(TRIANGLE)
{
    bindData();
}

Mesh::Mesh(std::vector<glm::vec3> vertex, std::vector<glm::vec3> colours, Shape shape) :
    modelMatrix(1.f),
    m_vertexes(std::move(vertex)),
    m_colours(std::move(colours)),
    m_type(COLOUR),
    m_shape(shape)
{
    bindData();
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_vertexArray);
    m_vertexArray = 0;
}

void Mesh::addTexture(Texture texture)
{
    m_textures.push_back(std::move(texture));
}

void Mesh::addAnimation(Animation &&animation)
{
    m_animations.push_back(std::move(animation));
}

void Mesh::setAnimation(int id)
{
    m_animIndex = id >= 0 && id < (int)m_animations.size() ? id : -1;
    if(m_animIndex != -1)
    {
        m_animations[m_animIndex].prepare();
    }
}


void Mesh::render(Program &program)
{
    glBindVertexArray(m_vertexArray);

    glm::mat4 tmp = modelMatrix;
    if(m_animIndex != -1 && (int)m_animations.size() > m_animIndex)
    {
        tmp *= m_animations[m_animIndex].transform();
    }

    program.setUniform(glsl_model_matrix, tmp);

    if(program.type() != Selection)
    {
        if(m_type == UV)
        {
            for(unsigned i = 0; i < m_textures.size(); ++i)
            {
                program.setUniform(glsl_sampler[i], i);
                m_textures[i].bind(i);
            }

            program.setUniform(glsl_texture_count,(int) m_textures.size());
        }
    }

    switch (m_shape)
    {
        case LINE:
            glDrawArrays(GL_LINES, 0, m_vertexes.size());
            break;
        case TRIANGLE:
            glDrawArrays(GL_TRIANGLES, 0, m_vertexes.size());
            break;
        default:
            break;
    }

    glBindVertexArray(0);

}


Mesh &Mesh::operator =(Mesh &&other)
{
    modelMatrix = std::move(other.modelMatrix);
    m_vertexArray = other.m_vertexArray;
    m_vertexes = std::move(other.m_vertexes);
    m_normals = std::move(other.m_colours);
    m_colours = std::move(other.m_normals);
    m_uvs = std::move(other.m_uvs);
    m_textures = std::move(other.m_textures);
    m_animations = std::move(other.m_animations);
    m_animIndex = other.m_animIndex;
    m_type = other.m_type;
    m_shape = other.m_shape;

    other.m_vertexArray = 0;
    other.m_animIndex = -1;
    return *this;
}

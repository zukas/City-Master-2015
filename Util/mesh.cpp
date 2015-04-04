#include "mesh.h"
#include "clock.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>


struct Private
{

        std::vector<glm::vec3 > m_vertexes;
        std::vector<glm::vec3 > m_normals;
        std::vector<glm::vec3 > m_colours;
        std::vector<glm::vec2 > m_uvs;

        Private(std::vector<glm::vec3 > vertexes, std::vector<glm::vec3 > normals, std::vector<glm::vec3 > colours, std::vector<glm::vec2 > uvs):
            m_vertexes(std::move(vertexes)),
            m_normals(std::move(normals)),
            m_colours(std::move(colours)),
            m_uvs(std::move(uvs)) {}
};

void Mesh::bindData()
{
    glGenVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);

    GLID buffer;
    glGenBuffers(1, &buffer);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, m_data.size() * sizeof(MeshUVPoint), &m_data[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
                0,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                sizeof(MeshUVPoint),                  // stride
                0           // array buffer offset
                );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
                1,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                sizeof(MeshUVPoint),                  // stride
                reinterpret_cast<void* >(sizeof(glm::vec3))            // array buffer offset
                );

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
                2,                  // attribute
                2,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                sizeof(MeshUVPoint),                  // stride
                reinterpret_cast<void* >(sizeof(glm::vec3)  * 2)            // array buffer offset
                );
    glBindVertexArray(0);
}

void Mesh::cleanUp()
{
    glDeleteVertexArrays(1, &m_vertexArray);
    m_vertexArray = 0;
}

Mesh::Mesh()
{

}

Mesh::Mesh(Mesh &&other) :
    m_vertexArray(other.m_vertexArray),
    modelMatrix(std::move(other.modelMatrix)),
    m_data(other.m_data),
    m_textures(std::move(other.m_textures)),
    m_animations(std::move(other.m_animations)),
    m_animIndex(other.m_animIndex)
{

    other.m_vertexArray = 0;
    other.m_animIndex = -1;
}

Mesh::Mesh(const Mesh &other) :
    modelMatrix(other.modelMatrix),
    m_data(other.m_data),
    m_textures(other.m_textures),
    m_animations(other.m_animations),
    m_animIndex(other.m_animIndex)
{
    bindData();
}

Mesh::Mesh(std::vector<MeshUVPoint> &&data) :
    modelMatrix(1.f),
    m_data(std::move(data))
{
    bindData();
}

Mesh::~Mesh()
{
    cleanUp();
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

    program.setModelMatrix(tmp);

    if(program.type() != Selection)
    {
        unsigned text_c =  m_textures.size();
        program.setSampers(text_c);
        for(unsigned i = 0; i < text_c; ++i)
        {
            m_textures[i].bind(i);
        }
    }

    glDrawArrays(GL_TRIANGLES, 0, m_data.size());


    glBindVertexArray(0);

}


Mesh &Mesh::operator = (Mesh &&other)
{
    cleanUp();

    modelMatrix = std::move(other.modelMatrix);
    m_vertexArray = other.m_vertexArray;
    m_data = std::move(other.m_data),
    m_textures = std::move(other.m_textures);
    m_animations = std::move(other.m_animations);
    m_animIndex = other.m_animIndex;

    other.m_vertexArray = 0;
    other.m_animIndex = -1;

    return *this;
}

Mesh &Mesh::operator = (const Mesh &other)
{
    cleanUp();

    modelMatrix = other.modelMatrix;
    m_data = other.m_data;
    m_textures = other.m_textures;
    m_animations = other.m_animations;

    bindData();

    return *this;
}

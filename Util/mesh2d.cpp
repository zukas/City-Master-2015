#include "mesh2d.h"

#include <GL/glew.h>

void Mesh2d::cleanUp()
{
    glDeleteVertexArrays(1, &m_vertexArray);
    m_vertexArray = 0;
}

Mesh2d::Mesh2d(Mesh2d &&other) :
    m_vertexArray(other.m_vertexArray),
    m_dataSize(other.m_dataSize),
    m_indexes(std::move(other.m_indexes)),
    m_textures(std::move(other.m_textures))
{
    other.m_dataSize = 0;
    other.m_vertexArray = 0;
}


Mesh2d::Mesh2d(const Mesh2d &other) :
    m_dataSize(other.m_dataSize),
    m_indexes(other.m_indexes),
    m_textures(other.m_textures)
{

}


Mesh2d::Mesh2d(size_t dataSize) :
    m_dataSize(std::move(dataSize))
{

}

Mesh2d::~Mesh2d()
{
    cleanUp();
}


void Mesh2d::bindDataIndexed(std::vector<vertex_2d> &data)
{
    glGenVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);

    GLID buffer[2];
    glGenBuffers(2, buffer);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, m_dataSize * sizeof(vertex_2d), &data[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
                0,                  // attribute
                2,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                sizeof(vertex_2d),                  // stride
                0           // array buffer offset
                );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
                1,                  // attribute
                2,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                sizeof(vertex_2d),                  // stride
                reinterpret_cast<void* >(sizeof(vec2))            // array buffer offset
                );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer[1] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_indexes.size() * sizeof(GLID), &m_indexes[0], GL_STATIC_DRAW );
    glBindVertexArray(0);
}


void Mesh2d::bindDataNotIndexed(std::vector<vertex_2d> &data)
{
    glGenVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);

    GLID buffer[2];
    glGenBuffers(1, buffer);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, m_dataSize * sizeof(vertex_2d), &data[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
                0,                  // attribute
                2,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                sizeof(vertex_2d),                  // stride
                0           // array buffer offset
                );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
                1,                  // attribute
                2,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                sizeof(vertex_2d),                  // stride
                reinterpret_cast<void* >(sizeof(vec2))            // array buffer offset
                );
    glBindVertexArray(0);
}


void Mesh2d::addTexture(Texture texture)
{
    m_textures.push_back(std::move(texture));
}


int Mesh2d::textureCount() const
{
    return m_textures.size();
}


void Mesh2d::renderIndexed(Program &program)
{
    glBindVertexArray(m_vertexArray);

    if(program.type() != Selection)
    {
        unsigned text_c =  m_textures.size();
        program.setSampers(text_c);
        for(unsigned i = 0; i < text_c; ++i)
        {
            m_textures[i].bind(i);
        }
    }

    glDrawElements(GL_TRIANGLES, m_indexes.size(), GL_UNSIGNED_INT, nullptr);

    if(program.type() != Selection)
    {
        unsigned text_c =  m_textures.size();
        program.setSampers(text_c);
        for(unsigned i = 0; i < text_c; ++i)
        {
            m_textures[i].unbind(i);
        }
    }

    glBindVertexArray(0);
}


void Mesh2d::renderNotIndexed(Program &program)
{
    glBindVertexArray(m_vertexArray);

    if(program.type() != Selection)
    {
        unsigned text_c =  m_textures.size();
        program.setSampers(text_c);
        for(unsigned i = 0; i < text_c; ++i)
        {
            m_textures[i].bind(i);
        }
    }

    glDrawArrays(GL_TRIANGLES, 0, m_dataSize);

    if(program.type() != Selection)
    {
        unsigned text_c =  m_textures.size();
        program.setSampers(text_c);
        for(unsigned i = 0; i < text_c; ++i)
        {
            m_textures[i].unbind(i);
        }
    }

    glBindVertexArray(0);
}

Mesh2d &Mesh2d::operator =(Mesh2d &&other)
{
    cleanUp();

    m_vertexArray = other.m_vertexArray;
    m_dataSize = other.m_dataSize;
    m_indexes = std::move(other.m_indexes);
    m_textures = std::move(other.m_textures);

    other.m_vertexArray = 0;

    return *this;
}

Mesh2d &Mesh2d::operator =(const Mesh2d &other)
{
    cleanUp();

    m_dataSize = other.m_dataSize;
    m_indexes = other.m_indexes;
    m_textures = other.m_textures;

    return *this;
}

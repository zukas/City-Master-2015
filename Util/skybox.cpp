#include "skybox.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

constexpr float size { 99.f };
//constexpr float vertexs[72]
//{
//    size, size, size,
//    size, -size, size,
//    -size, size, size,
//    -size, -size, size,
//    -size, size, -size,
//    -size, -size, -size,
//    size, size, -size,
//    size, -size, -size,
//    -size, size, size,
//    -size, -size, size,
//    -size, size, -size,
//    -size, -size, -size,
//    size, size, -size,
//    size, -size, -size,
//    size, size, size,
//    size, -size, size,
//    -size, size, -size,
//    size, size, -size,
//    -size, size, size,
//    size, size, size,
//    size, -size, -size,
//    -size, -size, -size,
//    size, -size, size,
//    -size, -size, size
//};

constexpr float vertexes[]
{
    -10.0f,  10.0f, -10.0f,
      -10.0f, -10.0f, -10.0f,
       10.0f, -10.0f, -10.0f,
       10.0f, -10.0f, -10.0f,
       10.0f,  10.0f, -10.0f,
      -10.0f,  10.0f, -10.0f,

      -10.0f, -10.0f,  10.0f,
      -10.0f, -10.0f, -10.0f,
      -10.0f,  10.0f, -10.0f,
      -10.0f,  10.0f, -10.0f,
      -10.0f,  10.0f,  10.0f,
      -10.0f, -10.0f,  10.0f,

       10.0f, -10.0f, -10.0f,
       10.0f, -10.0f,  10.0f,
       10.0f,  10.0f,  10.0f,
       10.0f,  10.0f,  10.0f,
       10.0f,  10.0f, -10.0f,
       10.0f, -10.0f, -10.0f,

      -10.0f, -10.0f,  10.0f,
      -10.0f,  10.0f,  10.0f,
       10.0f,  10.0f,  10.0f,
       10.0f,  10.0f,  10.0f,
       10.0f, -10.0f,  10.0f,
      -10.0f, -10.0f,  10.0f,

      -10.0f,  10.0f, -10.0f,
       10.0f,  10.0f, -10.0f,
       10.0f,  10.0f,  10.0f,
       10.0f,  10.0f,  10.0f,
      -10.0f,  10.0f,  10.0f,
      -10.0f,  10.0f, -10.0f,

      -10.0f, -10.0f, -10.0f,
      -10.0f, -10.0f,  10.0f,
       10.0f, -10.0f, -10.0f,
       10.0f, -10.0f, -10.0f,
      -10.0f, -10.0f,  10.0f,
       10.0f, -10.0f,  10.0f
};

constexpr float vertexs[108]
{
    size, -size, -size, size, -size,  size, size,  size,  size, size,  size,  size, size,  size, -size, size, -size, -size,
    -size, -size,  size,-size, -size, -size,-size,  size, -size,-size,  size, -size,-size,  size,  size,-size, -size,  size,
    -size,  size, -size, size,  size, -size, size,  size,  size, size,  size,  size,-size,  size,  size,-size,  size, -size,
    -size, -size,  size, size, -size,  size, size, -size, -size, size, -size, -size,-size, -size, -size,-size, -size,  size,
    size, -size,  size,-size, -size,  size,-size,  size,  size,-size,  size,  size, size,  size,  size, size, -size,  size,
    -size, -size, -size, size, -size, -size, size,  size, -size, size,  size, -size,-size,  size, -size,-size, -size, -size
};

constexpr float normals[72]
{
    0.00f, 0.00f, -1.00f,
    0.00f, 0.00f, -1.00f,
    0.00f, 0.00f, -1.00f,
    0.00f, 0.00f, -1.00f,
    0.00f, 0.00f, 1.00f,
    0.00f, 0.00f, 1.00f,
    0.00f, 0.00f, 1.00f,
    0.00f, 0.00f, 1.00f,
    1.00f, 0.00f, 0.00f,
    1.00f, 0.00f, 0.00f,
    1.00f, 0.00f, 0.00f,
    1.00f, 0.00f, 0.00f,
    -1.00f, 0.00f, 0.00f,
    -1.00f, 0.00f, 0.00f,
    -1.00f, 0.00f, 0.00f,
    -1.00f, 0.00f, 0.00f,
    0.00f, -1.00f, 0.00f,
    0.00f, -1.00f, 0.00f,
    0.00f, -1.00f, 0.00f,
    0.00f, -1.00f, 0.00f,
    0.00f, 1.00f, 0.00f,
    0.00f, 1.00f, 0.00f,
    0.00f, 1.00f, 0.00f,
    0.00f, 1.00f, 0.00f
};
constexpr float uvs[48]
{
    0.00f, 1.00f,
    0.00f, 0.00f,
    1.00f, 1.00f,
    1.00f, 0.00f,
    0.00f, 1.00f,
    0.00f, 0.00f,
    1.00f, 1.00f,
    1.00f, 0.00f,
    0.00f, 1.00f,
    0.00f, 0.00f,
    1.00f, 1.00f,
    1.00f, 0.00f,
    0.00f, 1.00f,
    0.00f, 0.00f,
    1.00f, 1.00f,
    1.00f, 0.00f,
    0.00f, 1.00f,
    0.00f, 0.00f,
    1.00f, 1.00f,
    1.00f, 0.00f,
    0.00f, 1.00f,
    0.00f, 0.00f,
    1.00f, 1.00f,
    1.00f, 0.00f
};

SkyBox::SkyBox()
{

}

SkyBox::SkyBox(const std::string &right, const std::string &left, const std::string &bottom, const std::string &top, const std::string &front, const std::string &back) :
    m_texture(right, left, bottom, top, front, back)
{

    m_texture.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_texture.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_texture.setSamplerParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


    glGenVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);
    GLID buffer;
    glGenBuffers(1, &buffer);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(float) ,vertexs, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
                0,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                nullptr           // array buffer offset
                );
    //    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    //    glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(float,&normals[0], GL_STATIC_DRAW);
    //    glEnableVertexAttribArray(1);
    //    glVertexAttribPointer(
    //                1,                  // attribute
    //                3,                  // size
    //                GL_FLOAT,           // type
    //                GL_FALSE,           // normalized?
    //                0,                  // stride
    //                0           // array buffer offset
    //                );

    //    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    //    glBufferData(GL_ARRAY_BUFFER, 48 * sizeof(float,&uvs[0], GL_STATIC_DRAW);
    //    glEnableVertexAttribArray(2);
    //    glVertexAttribPointer(
    //                2,                  // attribute
    //                2,                  // size
    //                GL_FLOAT,           // type
    //                GL_FALSE,           // normalized?
    //                0,                  // stride
    //                0           // array buffer offset
    //                );

    glBindVertexArray(0);
}

SkyBox::~SkyBox()
{

}

void SkyBox::render(Program &program)
{
    glDepthMask (GL_FALSE);
    glBindVertexArray(m_vertexArray);
//    program.setSampers(1);
//    m_texture.bind(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask (GL_TRUE);
}


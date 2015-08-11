#include "box2d.h"

#include <GL/glew.h>
#include <GL/gl.h>

constexpr auto vs = VS(uniform mat4 modelMatrix; uniform mat4 projectionMatrix;

               layout(location = 0) in vec2 inPosition;
               layout(location = 1) in vec2 inCoord;

               out vec2 texCoord;

               void main() {
                       gl_Position = projectionMatrix * modelMatrix *
                                                     vec4(inPosition, 0.0, 1.0);
                       texCoord = inCoord;
               });

constexpr auto fs = FS(in vec2 texCoord; out vec4 outColour;

              uniform sampler2D tsampler[1];

              void main() {
                    outColour = texture2D(tsampler[0], texCoord);
//                    outColour = vec4(1,1,1,1);
          });


Box2d::Box2d()
{

}

void Box2d::init(unsigned char *buff, size_t size, float height, float width)
{
    m_height = height;
    m_width = width;
    m_texture = Texture(GL_TEXTURE_2D, buff, size);

    m_data = new box2d_data[6]
    {
        {{0.f, 0.f}, {0.f, 0.f}},
        {{width, 0.f}, {1.f, 0.f}},
        {{0.f, height}, {0.f, 1.f}},
        {{width, height}, {1.f, 1.f}},
        {{0.f, height}, {0.f, 1.f}},
        {{width, 0.f}, {1.f, 0.f}}
    };
    glGenVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);
    GLID buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(box2d_data) * 6, m_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
                0,                  // attribute
                2,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                sizeof(box2d_data),                  // stride
                nullptr           // array buffer offset
                );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
                1,                  // attribute
                2,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                sizeof(box2d_data),                  // stride
                reinterpret_cast<void* >(sizeof(float) *2)            // array buffer offset
                );

    glBindVertexArray(0);

    m_program.createShader(vs);
    m_program.createShader(fs);
    m_program.createProgram();
    m_program.linkProgram();
    m_program.resolveUniforms();

}

void Box2d::render(float x, float y)
{
    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(m_vertexArray);
    m_program.use();
    m_program.setSampers(1);

    auto viewport = m_program.getViewport();
    m_program.setProjectionMatrix(
        glm::ortho(0.0f, float(viewport.w), 0.0f, float(viewport.h)));
    m_texture.bind(0);
    glm::mat4 model = glm::translate(glm::mat4(1.0f),
                       glm::vec3(float(x), float(y), 0.0f));

    m_program.setModelMatrix(model);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisable(GL_BLEND);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

#include "text.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype.h>
#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/gtc/matrix_transform.hpp>


inline int next_size(int size)
{
    int res = 1;
    while(res < size)
        res <<= 1;
    return res;
}

void load_char(char_data &data, FT_Face face, int index)
{

    FT_Load_Glyph(face, FT_Get_Char_Index(face, index), FT_LOAD_DEFAULT);
    FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    FT_Bitmap* bitmap = &face->glyph->bitmap;

    int width = bitmap->width;
    int height = bitmap->rows;
    int next_width = next_size(width);
    int next_heigth = next_size(height);

    std::vector<unsigned char > buffer(next_width*next_heigth);
    for(int i = 0; i < next_heigth; ++i)
    {
        for(int j = 0; j < next_width; ++j)
        {
            buffer[i*next_width + j] = (i >= height || j >= width) ? 0 : bitmap->buffer[(height -i - 1)*width+j];
        }
    }

    data.c = Texture { GL_TEXTURE_2D, buffer, next_width, next_heigth, GL_DEPTH_COMPONENT };
    data.c.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    data.c.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    data.c.setSamplerParameter(GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);

    data.aX = face->glyph->advance.x>>6;
    data.bX = face->glyph->metrics.horiBearingX>>6;
    data.cW = face->glyph->metrics.width>>6;

    data.aY = (face->glyph->metrics.height - face->glyph->metrics.horiBearingY)>>6;
    data.bY = face->glyph->metrics.horiBearingY>>6;
    data.cH = face->glyph->metrics.height>>6;


    data.pos[0] = glm::vec2(0.0f, float(-data.aY+next_heigth));
    data.pos[1] = glm::vec2(0.0f, float(-data.aY));
    data.pos[2] = glm::vec2(float(next_width), float(-data.aY+next_heigth));
    data.pos[3] = glm::vec2(float(next_width), float(-data.aY));

    data.coord[0] = glm::vec2(0.0f, 1.0f);
    data.coord[1] = glm::vec2(0.0f, 0.0f);
    data.coord[2] = glm::vec2(1.0f, 1.0f);
    data.coord[3] = glm::vec2(1.0f, 0.0f);
}


RefCount Text::g_counter {};

Text::Text()
{

}

Text::Text(const std::string &font, int size) :
    m_fontSize(size)
{
    FT_Library lib { nullptr };
    FT_Face face { nullptr };
    FT_Init_FreeType(&lib);
    FT_New_Face(lib, font.c_str(), 0, &face);
    FT_Set_Pixel_Sizes(face, size, size);

    glGenVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);

    std::vector<glm::vec2 > position ( 256 * 4);
    std::vector<glm::vec2 > coordiante ( 256 * 4);

    for(int i = 0; i < 256; ++i)
    {
        m_data[i].pos = &position[i * 4];
        m_data[i].coord = &coordiante[i * 4];
        load_char(m_data[i], face, i);
        m_newLine = std::max(m_newLine, m_data[i].cH);
        m_data[i].pos = nullptr;
        m_data[i].coord = nullptr;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(lib);

    GLuint buffers[2];
    glGenBuffers(2, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(glm::vec2), &position[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
                0,                  // attribute
                2,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                0           // array buffer offset
                );

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, coordiante.size() * sizeof(glm::vec2), &coordiante[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
                1,                  // attribute
                2,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                0           // array buffer offset
                );

    g_counter + m_vertexArray;

    glBindVertexArray(0);
}

Text::Text(Text &&other) :
    m_vertexArray(other.m_vertexArray),
    m_data(std::move(other.m_data)),
    m_newLine(other.m_newLine),
    m_fontSize(other.m_fontSize)
{
    other.m_vertexArray = 0;
}

Text::~Text()
{
    if(m_vertexArray != 0 && (g_counter - m_vertexArray) == 0)
    {
        glDeleteVertexArrays(1, &m_vertexArray);
    }
}

Text &Text::operator =(Text &&other)
{
    m_vertexArray = other.m_vertexArray;
    m_data = std::move(other.m_data);
    m_newLine = other.m_newLine;
    m_fontSize = other.m_fontSize;
    other.m_vertexArray = 0;
    return *this;
}

void Text::render(Program &program, const glm::vec4 &colour, const std::string &text, int x, int y, int size)
{
    glBindVertexArray(m_vertexArray);
    program.setSampers(1);
//    program.setUniform(glsl_sampler[0], 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int cursorX = x;
    int cursorY = program.getViewport().h - y;

    if(size == -1)
    {
        size = m_fontSize;
    }
    float scale = float(size)/float(m_fontSize);
    for(auto &c : text)
    {
        if(c == '\n')
        {
            cursorX = x;
            cursorY -= m_newLine* scale;
            continue;
        }
        int index = int(c);
        cursorX += m_data[index].bX * scale;
        if(c != ' ')
        {
            m_data[index].c.bind(0);

            glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(float(cursorX), float(cursorY), 0.0f)),glm::vec3(scale));

            program.setModelMatrix(model);
            program.setColour(colour);

//            program.setUniform(glsl_model_matrix, model);
//            program.setUniform(glsl_colour, colour);
            // Draw character
            glDrawArrays(GL_TRIANGLE_STRIP, index*4, 4);
        }

        cursorX += (m_data[index].aX - m_data[index].bX) * scale;
    }
    glDisable(GL_BLEND);
}


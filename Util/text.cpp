#include "text.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <cstring>

#include <glm/gtc/matrix_transform.hpp>


constexpr auto vs = VS(
						uniform mat4 modelMatrix;
						uniform mat4 projectionMatrix;

						layout (location = 0) in vec2 inPosition;
						layout (location = 1) in vec2 inCoord;

						out vec2 texCoord;

						void main()
						{
							gl_Position = projectionMatrix*modelMatrix*vec4(inPosition, 0.0, 1.0);
							texCoord = inCoord;
						}
);

constexpr auto fs = FS(
						in vec2 texCoord;
						out vec4 outColour;

						uniform sampler2D tsampler[1];
						uniform vec4 colour;

						void main()
						{
							vec4 textColour = texture2D(tsampler[0], texCoord);
							outColour = vec4(textColour.r, textColour.r, textColour.r, textColour.r) * colour;
						}
);



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

	data.aX = face->glyph->advance.x>>6;
	data.bX = face->glyph->metrics.horiBearingX>>6;
	data.cW = face->glyph->metrics.width>>6;

	data.aY = (face->glyph->metrics.height - face->glyph->metrics.horiBearingY)>>6;
	data.bY = face->glyph->metrics.horiBearingY>>6;
	data.cH = face->glyph->metrics.height>>6;

	FT_Bitmap* bitmap = &face->glyph->bitmap;
	int width = bitmap->width;
	int height = bitmap->rows;
	int next_width = next_size(data.aX);
	int next_heigth = next_size(data.cH);

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

Text::Text(const char *font, int size) :
	m_fontSize(size)
{
	FT_Library lib { nullptr };
	FT_Face face { nullptr };
	FT_Init_FreeType(&lib);
	FT_New_Face(lib, font, 0, &face);
	FT_Set_Pixel_Sizes(face, size, size);

	glGenVertexArrays(1, &m_vertexArray);
	glBindVertexArray(m_vertexArray);

	glm::vec2 position [ char_data_set * 4 ];
	glm::vec2 coordiante [ char_data_set * 4 ];

	for(int i = 0; i < char_data_set; ++i)
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
	glBufferData(GL_ARRAY_BUFFER, char_data_set * 4 * sizeof(glm::vec2), position, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
				0,                  // attribute
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				nullptr           // array buffer offset
				);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, char_data_set * 4 * sizeof(glm::vec2), coordiante, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
				1,                  // attribute
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				nullptr         // array buffer offset
				);

	g_counter + m_vertexArray;

	glBindVertexArray(0);

	m_program.createShader(vs);
	m_program.createShader(fs);
	m_program.createProgram();
	m_program.linkProgram();
    m_program.resolveUniforms();
}

Text::Text(unsigned char *font_buffer, size_t font_bufer_size, int size) :
    m_fontSize(size)
{
    FT_Library lib { nullptr };
    FT_Face face { nullptr };
    FT_Init_FreeType(&lib);
    FT_New_Memory_Face(lib, font_buffer, font_bufer_size, 0, &face);
    FT_Set_Pixel_Sizes(face, size, size);

    glGenVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);

    glm::vec2 position [ char_data_set * 4 ];
    glm::vec2 coordiante [ char_data_set * 4 ];

    for(int i = 0; i < char_data_set; ++i)
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
    glBufferData(GL_ARRAY_BUFFER, char_data_set * 4 * sizeof(glm::vec2), position, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
                0,                  // attribute
                2,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                nullptr           // array buffer offset
                );

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, char_data_set * 4 * sizeof(glm::vec2), coordiante, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
                1,                  // attribute
                2,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                nullptr         // array buffer offset
                );

    g_counter + m_vertexArray;

    glBindVertexArray(0);

    m_program.createShader(vs);
    m_program.createShader(fs);
    m_program.createProgram();
    m_program.linkProgram();
    m_program.resolveUniforms();
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
	m_program = std::move(other.m_program);
	m_data = std::move(other.m_data);
	m_newLine = other.m_newLine;
	m_fontSize = other.m_fontSize;
	other.m_vertexArray = 0;
	return *this;
}

void Text::render(const char *text, const glm::vec4 &colour, int x, int y, int size)
{

	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(m_vertexArray);
	m_program.use();
	m_program.setSampers(1);

	auto viewport = m_program.getViewport();

	m_program.setProjectionMatrix(glm::ortho(0.0f, float(viewport.w), 0.0f, float(viewport.h)));

	int cursorX = x;
	int cursorY = viewport.h - y;



	if(size == -1)
	{
		size = m_fontSize;
	}
	float scale = float(size)/float(m_fontSize);
	float incY = m_newLine * scale;
	m_last = glm::vec2 { 0.f, incY };

	auto length = std::strlen(text);
	for(decltype(length) i = 0; i < length; ++i)
	{
		if(text[i] == '\n')
		{
			cursorX = x;
			cursorY -= incY;
			m_last.y += incY;
			continue;
		}
		int index = int(text[i]);
		cursorX += m_data[index].bX * scale;
		if(text[i] != ' ')
		{
			m_data[index].c.bind(0);

			glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(float(cursorX), float(cursorY), 0.0f )),glm::vec3(scale));

			m_program.setModelMatrix(model);
			m_program.setColour(colour);
			glDrawArrays(GL_TRIANGLE_STRIP, index * 4, 4);
		}

		cursorX += (m_data[index].aX - m_data[index].bX) * scale;
		m_last.x = std::max(m_last.x, float(cursorX));
	}
	glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

}

int Text::lineHeight() const
{
	return m_newLine;
}

glm::vec2 Text::lastDimentions() const
{
	return m_last;
}

Program &Text::program()
{
	return m_program;
}


#include "text.h"
#include "program.h"
#include "texture2dcollection.h"
#include "viewport.h"
#include "profiler.h"
#include "glvalidator.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype.h>
#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/gtc/matrix_transform.hpp>

inline uint32_t next_size(uint32_t size) {
	uint32_t res = 1;
	while (res < size)
		res <<= 1;
	return res;
}

inline uint32_t max(uint32_t a, uint32_t b) { return a > b ? a : b; }

struct char_detail_t {
	char_data_t char_data;
	uint32_t height;
};

char_detail_t load_char(float *buffer_data, FT_Face face, uint32_t index) {

	char_detail_t res;
	FT_Load_Glyph(face, FT_Get_Char_Index(face, index), FT_LOAD_DEFAULT);
	FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
	const FT_Bitmap *bitmap = &face->glyph->bitmap;

	const uint16_t ax = face->glyph->advance.x >> 6;
	const uint16_t bx = face->glyph->metrics.horiBearingX >> 6;
	const uint32_t nw = next_size(ax);

	const uint16_t ay =
		(face->glyph->metrics.height - face->glyph->metrics.horiBearingY) >> 6;
	const uint16_t ch = face->glyph->metrics.height >> 6;
	const uint32_t nh = next_size(ch);

	const uint32_t width = bitmap->width;
	const uint32_t height = bitmap->rows;
	GL_CHECK;
	{
		byte buffer[32768]{0};
		ASSERT(nw * nh < 32768);

		const byte *bitmap_buffer = bitmap->buffer;

		for (uint32_t i = 0; i < nh; ++i) {
			for (uint32_t j = 0; j < nw; ++j) {
				buffer[i * nw + j] =
					(i >= height || j >= width)
						? 0
						: bitmap_buffer[(height - i - 1) * width + j];
			}
		}

		res.char_data.texture_id =
			Texture2DCollection::create_ttf_from_memory(buffer, nw, nh);
		res.char_data.advance = ax;
		res.char_data.bearing = bx;
		res.height = ch;
		GL_CHECK;
	}

	const float _hy = float(int32_t(nh - ay));
	const float _nw = float(int32_t(nw));
	const float _ay = float(int32_t(-ay));

	buffer_data[0] = 0.f;
	buffer_data[1] = _hy;
	buffer_data[2] = 0.f;
	buffer_data[3] = 1.f;

	buffer_data[4] = 0.f;
	buffer_data[5] = _ay;
	buffer_data[6] = 0.f;
	buffer_data[7] = 0.f;

	buffer_data[8] = _nw;
	buffer_data[9] = _hy;
	buffer_data[10] = 1.f;
	buffer_data[11] = 1.f;

	buffer_data[12] = _nw;
	buffer_data[13] = _ay;
	buffer_data[14] = 1.f;
	buffer_data[15] = 0.f;
	GL_CHECK;
	return res;
}

Text::program_data Text::text_program{0, 0, 0, 0, 0};

void Text::init(const ShaderSource *shaders, uint32_t size, uint32_t projection_hash,
				uint32_t model_hash, uint32_t colour_hash) {
	text_program.program_id = ProgramCompiler::compileProgram(shaders, size);
	uint32_t uniforms[4];
	ProgramCompiler::resolveUniforms(text_program.program_id, uniforms, 4);

	for (uint32_t i = 0; i < 4; ++i) {
		if (uniforms[i] == projection_hash) {
			text_program.projection_id = i;
			continue;
		}

		if (uniforms[i] == model_hash) {
			text_program.model_id = i;
			continue;
		}

		if (uniforms[i] == colour_hash) {
			text_program.colour_id = i;
			continue;
		}
	}
}

void Text::beginRender() {

	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(text_program.program_id);
	Uniforms::setUniform(text_program.projection_id, Viewport::ortho());
	GL_CHECK;
}

void Text::endRender() {
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	GL_CHECK;
}

Text::Text() {}

Text::Text(byte *font_buffer, ulong font_bufer_size, uint32_t size) {
	GL_CHECK;
	FT_Library lib{nullptr};
	FT_Face face{nullptr};
    FT_Init_FreeType(&lib);
    FT_New_Memory_Face(lib, font_buffer, font_bufer_size, 0, &face);
    FT_Set_Pixel_Sizes(face, size, size);

    glGenVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);
	GL_CHECK;
	float vertex_data[2048];

	for (uint32_t i = 0; i < 128; ++i) {
		char_detail_t detail = load_char(&vertex_data[i * 16], face, i);
		m_charData[i] = detail.char_data;
		m_lineSize = max(m_lineSize, detail.height);
	}
	GL_CHECK;
    FT_Done_Face(face);
    FT_Done_FreeType(lib);
	GL_CHECK;
	GLuint buffer;
	glGenBuffers(1, &buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data,
				 GL_STATIC_DRAW);
	GL_CHECK;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,                          // attribute
						  2,                          // size
						  GL_FLOAT,                   // type
						  GL_FALSE,                   // normalized?
						  sizeof(float[4]),           // stride
						  reinterpret_cast<void *>(0) // array buffer offset
						  );
	GL_CHECK;
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                                         // attribute
		2,                                         // size
		GL_FLOAT,                                  // type
		GL_FALSE,                                  // normalized?
		sizeof(float[4]),                          // stride
		reinterpret_cast<void *>(sizeof(float[2])) // array buffer offset
		);
	GL_CHECK;
    glBindVertexArray(0);
}

Text::~Text() {
	glDeleteVertexArrays(1, &m_vertexArray);
	m_vertexArray = 0;
}

void Text::setColour(const colour &colour_) { m_colour = colour_; }

uint32_t Text::render(const char *text, uint32_t x, uint32_t y) const {

	glBindVertexArray(m_vertexArray);
	Uniforms::setUniform(text_program.colour_id, m_colour);
	const uint32_t viewport_height = Viewport::viewport().h;
	const uint32_t incY = m_lineSize;
	const uint32_t length = strlen(text);

	uint32_t cursorX = x;
	uint32_t cursorY = viewport_height - y;

	for (uint32_t i = 0; i < length; ++i) {
		PROF("Render char");
		if (text[i] == '\n') {
			cursorX = x;
			cursorY -= incY;
			continue;
		}
		const int index = int(text[i]);
		const char_data_t char_ = m_charData[index];

		cursorX += char_.bearing;
		if (text[i] != ' ') {
			Texture2DCollection::bind(char_.texture_id);

			Uniforms::setUniform(
				text_program.model_id,
				glm::translate(
					glm::mat4(1.0f),
					glm::vec3(float(cursorX), float(cursorY), 0.0f)));
			glDrawArrays(GL_TRIANGLE_STRIP, index * 4, 4);
		}
		cursorX += (char_.advance - char_.bearing);
	}
	glBindVertexArray(0);

	return viewport_height - cursorY;
}

uint32_t Text::lineSize() const { return m_lineSize; }

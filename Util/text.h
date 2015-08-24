#ifndef TEXT_H
#define TEXT_H

#include "texture.h"
#include "program.h"

#include <array>
#include <glm/glm.hpp>

/**
 * @brief The char_data struct
 *
 */
struct char_data {
	Texture c{};
	int aX{0};
	int aY{0};
	int bX{0};
	int bY{0};
	int cW{0};
	int cH{0};
	glm::vec2 *pos{nullptr};
	glm::vec2 *coord{nullptr};
};

struct char_data_t {
	uint32_t texture_id;
	int32_t advance;
	int32_t bearing;
};

class Text {
	struct program_data {
		uint32_t program_id;
		uint32_t projection_id;
		uint32_t model_id;
		uint32_t colour_id;
		uint32_t texture_id;
	};

  private:
	static program_data text_program;

  private:
	GLID m_vertexArray{0};
	char_data_t m_charData[128];
	uint32_t m_lineSize{0};
	colour m_colour;

  public:
	static void init(const Shader *shaders, uint32_t size,
					 uint32_t projection_hash, uint32_t model_hash,
					 uint32_t colour_hash, uint32_t texture_hash);

	static void beginRender();
	static void endRender();

	Text();
	Text(Text &&) = default;
	Text(byte *font_buffer, ulong font_bufer_size, uint32_t size);
	~Text();
	void setColour(const colour &colour_);
	uint32_t render(const char *text, uint32_t x, uint32_t y) const;
	uint32_t lineSize() const;

	Text &operator=(Text &&) = default;
};

#endif // TEXT_H

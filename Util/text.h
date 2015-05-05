#ifndef TEXT_H
#define TEXT_H

#include "texture.h"
#include "program.h"

#include <array>
#include <glm/glm.hpp>

struct char_data
{
        Texture c {};
        int aX { 0 };
        int aY { 0 };
        int bX { 0 };
        int bY { 0 };
        int cW { 0 };
        int cH { 0 };
        glm::vec2 *pos { nullptr };
        glm::vec2 *coord { nullptr };
};

class Text
{
	private:
		static  constexpr int char_data_set { 128 };
    private:
        GLID m_vertexArray { 0 };
		Program m_program {};
		std::array<char_data, char_data_set > m_data;
        int m_newLine { 0 };
        int m_fontSize { 0 };
        static RefCount g_counter;

    public:
        Text();
		Text(const char *font, int size);
        Text(Text &&other);
        ~Text();

        Text &operator = (Text &&other);

		void render(const char *text, const glm::vec4 &colour, int x, int y, int size = -1);

		int lineHeight() const;
		Program &program();

        Text(const Text&) = delete;
        Text &operator = (const Text&) = delete;
};

#endif // TEXT_H

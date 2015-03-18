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
        GLID m_vertexArray { 0 };
        std::array<char_data, 256 > m_data;
        int m_newLine { 0 };
        int m_fontSize { 0 };

        static RefCount g_counter;

    public:
        Text();
        Text(const std::string &font, int size);
        Text(Text &&other);
        ~Text();

        Text &operator = (Text &&other);

        void render(Program &program, const glm::vec4 &colour,  const std::string &text, int x, int y, int size = -1);

        Text(const Text&) = delete;
        Text &operator = (const Text&) = delete;
};

#endif // TEXT_H

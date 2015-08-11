#ifndef BOX2D_H
#define BOX2D_H

#include "Util/texture.h"
#include "Util/program.h"

struct box2d_data
{
    float vertex[2];
    float uv[2];
};

class Box2d
{
private:
    GLID m_vertexArray { 0 };
    box2d_data *m_data;
    Program m_program;
    Texture m_texture;

    float m_height;
    float m_width;
public:
    Box2d();
    void init(unsigned char *buff, size_t size, float height, float width);
    void render(float x, float y);

};

#endif // BOX2D_H


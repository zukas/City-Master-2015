#ifndef BOX2D_H
#define BOX2D_H

#include "Util/texture2dcollection.h"

class Box2d
{
private:
    uint32_t m_model_id;
    uint32_t m_mesh;
    uint32_t m_texture;

    float m_height;
    float m_width;
public:
    Box2d();
    void init(uint32_t texture_id, float height, float width);
    void render(float x, float y);

};

#endif // BOX2D_H


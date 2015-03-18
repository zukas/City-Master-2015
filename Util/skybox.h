#ifndef SKYBOX_H
#define SKYBOX_H

#include "texture.h"
#include "mesh.h"

class SkyBox
{
    private:
        GLID m_vertexArray { 0 };
        Texture m_texture;

    public:
        SkyBox();
        SkyBox(const std::string &right, const std::string &left, const std::string &bottom, const std::string &top, const std::string &front, const std::string &back);
        ~SkyBox();

        void render(Program &program);
};

#endif // SKYBOX_H

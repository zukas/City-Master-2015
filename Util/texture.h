#ifndef TEXTURE_H
#define TEXTURE_H

#include "types.h"
#include "refcount.h"
#include <map>


class Texture
{
    private:
        struct TextureData
        {
                GLID m_textureID { 0 };
                GLID m_textureType { 0 };
                GLID m_samplerID { 0 };
        } m_data;

        std::string m_filename;
        static RefCount g_counter;
        static std::map<std::string, TextureData> g_data;

    public:
        Texture();
        Texture(GLID type, std::string file);
        Texture(GLID type, const std::vector<unsigned char > &buffer);
        Texture(const Texture &other) ;
        Texture(Texture &&other);
        ~Texture();
        bool bind(GLID unit);
        Texture &operator = (const Texture &other);
        Texture &operator = (Texture &&other);
};

#endif // TEXTURE_H

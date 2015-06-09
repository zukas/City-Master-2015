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

        long m_id { 0 };
        static RefCount g_counter;
        static std::map<long, TextureData> g_data;

    public:
        Texture();
        Texture(GLID type, const std::string &file);
        Texture(GLID type, const std::vector<unsigned char > &buffer, int width, int height, GLID format);
        Texture(const std::string &right, const std::string &left, const std::string &bottom, const std::string &top, const std::string &front, const std::string &back);
        Texture(const Texture &other) ;
        Texture(Texture &&other);
        void setSamplerParameter(GLID parameter, GLID value);
        ~Texture();
		bool bind(GLID unit) const;
		void unbind(GLID unit) const;
        Texture &operator = (const Texture &other);
        Texture &operator = (Texture &&other);
};

#endif // TEXTURE_H

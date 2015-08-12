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
				uint32_t m_textureID { 0 };
				uint32_t m_textureType { 0 };
				uint32_t m_samplerID { 0 };
        } m_data;

        long m_id { 0 };
        static RefCount g_counter;
        static std::map<long, TextureData> g_data;

    public:
        Texture();
		Texture(int size);
		Texture(unsigned type, const std::string &file);
		Texture(unsigned type, const unsigned char *data, size_t size);
		Texture(unsigned type, const std::vector<unsigned char > &buffer, int width, int height, unsigned format);
        Texture(const std::string &right, const std::string &left, const std::string &bottom, const std::string &top, const std::string &front, const std::string &back);
        Texture(const Texture &other) ;
        Texture(Texture &&other);
        void setSamplerParameter(GLID parameter, GLID value);
        ~Texture();
		bool bind(unsigned unit) const;
		void unbind(unsigned unit) const;
        Texture &operator = (const Texture &other);
        Texture &operator = (Texture &&other);
};



#endif // TEXTURE_H

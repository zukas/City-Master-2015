#include "texture.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <SOIL/SOIL.h>
#include <algorithm>


RefCount Texture::g_counter;
std::map<long, Texture::TextureData> Texture::g_data;

Texture::Texture()
{
}

Texture::Texture(GLID type, const std::string &file) :
    m_id(std::hash<std::string>()(file))
{
    auto it = g_data.find(m_id);
    if(it != std::end(g_data))
    {
        m_data = it->second;
    }
    else
    {
        m_data.m_textureType = type;
        m_data.m_textureID = SOIL_load_OGL_texture(
                    file.c_str(),
                    SOIL_LOAD_AUTO,
                    SOIL_CREATE_NEW_ID,
                    SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

        glTexParameterf(m_data.m_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(m_data.m_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenSamplers(1, &m_data.m_samplerID);
        g_data[m_id] = m_data;
    }


    g_counter + m_data.m_textureID;

}

Texture::Texture(GLID type, const std::vector<unsigned char> &buffer, int width, int height, GLID format)
{
    m_data.m_textureType = type;
    glGenTextures(1, &m_data.m_textureID);
    glBindTexture(m_data.m_textureType, m_data.m_textureID);
    glTexImage2D(m_data.m_textureType, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, buffer.data());
    glGenerateMipmap(m_data.m_textureType);
    glTexParameterf(m_data.m_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(m_data.m_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenSamplers(1, &m_data.m_samplerID);

    g_counter + m_data.m_textureID;
}

Texture::Texture(const std::string &right, const std::string &left, const std::string &bottom, const std::string &top, const std::string &front, const std::string &back)
{
    m_data.m_textureType = GL_TEXTURE_CUBE_MAP;
    m_data.m_textureID = SOIL_load_OGL_cubemap(
                             right.c_str(),
                             left.c_str(),
                             top.c_str(),
                             bottom.c_str(),
                             front.c_str(),
                             back.c_str(),
                             SOIL_LOAD_AUTO,
                             SOIL_CREATE_NEW_ID,
                             SOIL_FLAG_MIPMAPS);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if(GLEW_EXT_texture_filter_anisotropic)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 0);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenSamplers(1, &m_data.m_samplerID);

    g_counter + m_data.m_textureID;
}

Texture::Texture(const Texture &other) :
    m_data(other.m_data),
    m_id(other.m_id)
{
    g_counter+m_data.m_textureID;
}

Texture::Texture(Texture &&other) :
    m_data(other.m_data),
    m_id(std::move(other.m_id))
{
    other.m_data = {};
}

void Texture::setSamplerParameter(GLID parameter, GLID value)
{
    glSamplerParameteri(m_data.m_samplerID, parameter, value);
}

Texture::~Texture()
{
    if(m_data.m_textureID != 0 && g_counter - m_data.m_textureID == 0)
    {
        glDeleteTextures(1, &m_data.m_textureID);
        glDeleteSamplers(1, &m_data.m_samplerID);
        m_data = {};
        if(m_id)
        {
            g_data.erase(m_id);
        }
    }
}

bool Texture::bind(GLenum unit)
{
    if(m_data.m_textureID == 0) return false;
    glActiveTexture(GL_TEXTURE0+unit);
    glBindTexture(m_data.m_textureType, m_data.m_textureID);
    glBindSampler(unit, m_data.m_samplerID);
    return true;
}

Texture &Texture::operator = (const Texture &other)
{
    m_data = other.m_data;
    m_id = other.m_id;
    g_counter+m_data.m_textureID;

    return *this;
}

Texture &Texture::operator =(Texture &&other)
{
    m_data = other.m_data;
    m_id = std::move(other.m_id);
    other.m_data = {};
    return *this;
}

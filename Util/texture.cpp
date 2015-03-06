#include "texture.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <SOIL/SOIL.h>
#include <algorithm>


RefCount Texture::g_counter;
std::map<std::string, Texture::TextureData> Texture::g_data;

Texture::Texture()
{
}

Texture::Texture(GLID type, std::string file) :
    m_filename(std::move(file))
{

    auto it = g_data.find(m_filename);
    if(it != std::end(g_data))
    {
        m_data = it->second;
    }
    else
    {
        m_data.m_textureType = type;
        glGenTextures(1, &m_data.m_textureID);
        glBindTexture(m_data.m_textureType, m_data.m_textureID);
        int width { 0 };
        int height { 0 };
        auto data = SOIL_load_image(m_filename.c_str(), &width, &height, nullptr, SOIL_LOAD_RGB);
        glTexImage2D(m_data.m_textureType, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(m_data.m_textureType);
        glTexParameterf(m_data.m_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(m_data.m_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        SOIL_free_image_data(data);

        glGenSamplers(1, &m_data.m_samplerID);
        g_data[m_filename] = m_data;
    }


    g_counter + m_data.m_textureID;

}

Texture::Texture(GLID type, const std::vector<unsigned char> &buffer)
{
    m_data.m_textureType = type;
    glGenTextures(1, &m_data.m_textureID);
    glBindTexture(m_data.m_textureType, m_data.m_textureID);
    int width { 0 };
    int height { 0 };
    auto data = SOIL_load_image_from_memory(&buffer[0], buffer.size(), &width, &height, nullptr, SOIL_LOAD_RGB);
    glTexImage2D(m_data.m_textureType, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(m_data.m_textureType);
    glTexParameterf(m_data.m_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(m_data.m_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SOIL_free_image_data(data);

    glGenSamplers(1, &m_data.m_samplerID);

    g_counter + m_data.m_textureID;
}

Texture::Texture(const Texture &other) :
    m_data(other.m_data),
    m_filename(other.m_filename)
{
    g_counter+m_data.m_textureID;
}

Texture::Texture(Texture &&other) :
    m_data(other.m_data),
    m_filename(std::move(other.m_filename))
{
    other.m_data = {};
}

Texture::~Texture()
{
    if(m_data.m_textureID != 0 && g_counter - m_data.m_textureID == 0)
    {
        glDeleteTextures(1, &m_data.m_textureID);
        glDeleteSamplers(1, &m_data.m_samplerID);
        m_data = {};
        if(!m_filename.empty())
        {
            g_data.erase(m_filename);
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
    m_filename = other.m_filename;
    g_counter+m_data.m_textureID;

    return *this;
}

Texture &Texture::operator =(Texture &&other)
{
    m_data = other.m_data;
    m_filename = std::move(other.m_filename);
    other.m_data = {};
    return *this;
}

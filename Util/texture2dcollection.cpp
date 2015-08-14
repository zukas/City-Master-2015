#include "texture2dcollection.h"
#include "SOIL/SOIL.h"

#include <GL/glew.h>
#include <string.h>

uint32_t Texture2DCollection::_texture_size{0};
uint32_t Texture2DCollection::_sampler_size{0};

void Texture2DCollection::init(uint32_t texture_size, uint32_t sampler_size) {
    ASSERT(texture_size > 0);
    ASSERT(texture_size < MAX_TEXTURE_2D_COUNT);
    _texture_size = texture_size;

    ASSERT(sampler_size > 0);
    ASSERT(sampler_size < MAX_SAMPLER_SIZE);
    _sampler_size = sampler_size;

    uint32_t textures_[MAX_TEXTURE_2D_SIZE];
    glGenTextures(texture_size, textures_);

    uint32_t samplers_[MAX_SAMPLER_SIZE];
    glGenSamplers(sampler_size, samplers_);
}

void Texture2DCollection::destroy() {
    ASSERT(_texture_size > 0);
    ASSERT(_sampler_size > 0);

    const uint32_t tsize_ = _texture_size;
    const uint32_t ssize_ = _sampler_size;

    uint32_t textures_[MAX_TEXTURE_2D_SIZE];
    uint32_t samplers_[MAX_SAMPLER_SIZE];

    for (uint32_t i = 0; i < tsize_; ++i) {
        textures_[i] = i + 1;
    }

    for (uint32_t i = 0; i < ssize_; ++i) {
        samplers_[i] = i + 1;
    }

    glDeleteTextures(tsize_, textures_);
    glDeleteSamplers(ssize_, samplers_);
    _texture_size = 0;
    _sampler_size = 0;
}

uint32_t Texture2DCollection::texture_size() { return _texture_size; }

uint32_t Texture2DCollection::sampler_size() { return _sampler_size; }

void Texture2DCollection::cleate_dss_from_memory(uint32_t index, byte *buffer,
                                                 uint32_t size) {
    ASSERT(index >= 0);
    ASSERT(index < _texture_size);
    uint32_t texture_id_ = index + 1;
    texture_id_ = SOIL_load_OGL_texture_from_memory(
        buffer, size, SOIL_LOAD_AUTO, texture_id_, SOIL_FLAG_DDS_LOAD_DIRECT);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2DCollection::bind(const uint32_t *texture_indexes, uint32_t size) {
    ASSERT(texture_indexes != nullptr);
    ASSERT(size > 0);
    ASSERT(size < _sampler_size);
    for (uint32_t i = 0; i < size; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texture_indexes[i] + 1);
        glBindSampler(i, i);
    }
}

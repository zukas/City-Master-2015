#include "texture2dcollection.h"
#include "SOIL/SOIL.h"

#include <GL/glew.h>
#include <string.h>

uint32_t Texture2DCollection::_texture_size{0};

void Texture2DCollection::init(uint32_t texture_size) {
    ASSERT(texture_size > 0);
	ASSERT(texture_size <= MAX_TEXTURE_2D_SIZE);
    _texture_size = texture_size;

    uint32_t textures_[MAX_TEXTURE_2D_SIZE];
    glGenTextures(texture_size, textures_);
	glGenSamplers(texture_size, textures_);
	int a = 0;
	a = (1 +(a++))- 1;
}

void Texture2DCollection::destroy() {
    ASSERT(_texture_size > 0);

	const uint32_t size_ = _texture_size;

    uint32_t textures_[MAX_TEXTURE_2D_SIZE];

	for (uint32_t i = 0; i < size_; ++i) {
        textures_[i] = i + 1;
    }

	glDeleteTextures(size_, textures_);
	glDeleteSamplers(size_, textures_);
    _texture_size = 0;
}

uint32_t Texture2DCollection::texture_size() { return _texture_size; }

uint32_t Texture2DCollection::create_dss_from_memory(byte *buffer,
													 uint32_t size) {
	uint32_t texture_id_ = SOIL_load_OGL_texture_from_memory(
		buffer, size, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_DDS_LOAD_DIRECT);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture_id_;
}

uint32_t Texture2DCollection::create_ttf_from_memory(byte *buffer,
													 uint32_t width,
													 uint32_t height) {
	uint32_t texture_id_;

	glGenTextures(1, &texture_id_);
	glBindTexture(GL_TEXTURE_2D, texture_id_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
				 GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, buffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glSamplerParameteri(texture_id_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(texture_id_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(texture_id_, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture_id_;
}

void Texture2DCollection::bind(uint32_t texture_id, uint32_t unit) {
	glActiveTexture(GL_TEXTURE0 + unit);
	glUniform1i(texture_id, unit);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glBindSampler(unit, texture_id);
}

void Texture2DCollection::bind(const uint32_t *texture_ids, uint32_t size,
							   uint32_t start_unit) {
	ASSERT(texture_ids != nullptr);
    ASSERT(size > 0);
	ASSERT(size < _texture_size);
	const uint32_t size_ = size + start_unit;
	for (uint32_t i = start_unit; i < size_; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
		const uint32_t id = texture_ids[i];
		glUniform1i(id, i);
		glBindTexture(GL_TEXTURE_2D, id);
		glBindSampler(i, id);
    }
}

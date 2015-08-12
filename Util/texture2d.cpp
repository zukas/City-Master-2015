#include "texture2d.h"
#include <GL/glew.h>
#include <GL/gl.h>

#include "SOIL/SOIL.h"

Texture2D::~Texture2D() {
	glDeleteTextures(1, &m_texture_id);
	glDeleteSamplers(1, &m_sampler_id);
}

void Texture2D::init_dds_from_memory(byte *buffer, uint32_t size) {
	m_texture_id = SOIL_load_OGL_texture_from_memory(
		buffer, size, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_DDS_LOAD_DIRECT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenSamplers(1, &m_sampler_id);
}

void Texture2D::bind(GLID unit) const {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_texture_id);
	glBindSampler(unit, m_sampler_id);
}

Texture2D &&Texture2D::move() { return ::move(*this); }

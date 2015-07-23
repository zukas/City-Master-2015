#include "texturecollection2d.h"
#include <GL/glew.h>
#include <GL/gl.h>

#include "SOIL/SOIL.h"

texture2d TextureCollection2D::g_textures[50];

GLID TextureCollection2D::create_dds_from_memory(byte *buffer, uint32_t size) {

	GLID texture_id = SOIL_load_OGL_texture_from_memory(
		buffer, size, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_DDS_LOAD_DIRECT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	auto &texture_t = g_textures[texture_id - 1];
	ASSERT(texture_t.sampler_id == 0);
	texture_t.texture_id = texture_id;
	glGenSamplers(1, &texture_t.sampler_id);
	return texture_id;
}

void TextureCollection2D::destroy(GLID texture_id)
{
	ASSERT(texture_id > 0);
	ASSERT(texture_id <= 50);
	auto &texture_t = g_textures[texture_id - 1];
	ASSERT(texture_t.texture_id != 0);
	ASSERT(texture_t.sampler_id != 0);
	glDeleteTextures(1, &texture_t.texture_id);
	glDeleteSamplers(1, &texture_t.sampler_id);
	texture_t.texture_id = 0;
	texture_t.sampler_id = 0;
}

void TextureCollection2D::bind(GLID texture_id, GLID unit) {
	ASSERT(texture_id > 0);
	ASSERT(texture_id <= 50);
	auto texture_t = g_textures[texture_id - 1];

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture_t.texture_id);
	glBindSampler(unit, texture_t.sampler_id);
}

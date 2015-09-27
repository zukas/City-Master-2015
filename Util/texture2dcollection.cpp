#include "texture2dcollection.h"
#include "SOIL/SOIL.h"
#include "glvalidator.h"

#include <GL/glew.h>
#include <stdio.h>

uint32_t Texture2DCollection::create_dss_from_memory(byte *buffer,
													 uint32_t size) {
	//	uint32_t sampler_id_{0};
	uint32_t texture_id_ = SOIL_load_OGL_texture_from_memory(
		buffer, size, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_DDS_LOAD_DIRECT | SOIL_FLAG_MIPMAPS);
	GL_CHECK;
	glBindTexture(GL_TEXTURE_2D, 0);

	GL_CHECK;
	return texture_id_;
}

uint32_t Texture2DCollection::create_jpg_from_memory(byte *buffer,
													 uint32_t size) {
	uint32_t texture_id_ = SOIL_load_OGL_texture_from_memory(
		buffer, size, SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MIPMAPS);

	GL_CHECK;
	glBindTexture(GL_TEXTURE_2D, 0);

	GL_CHECK;
	return texture_id_;
}

uint32_t Texture2DCollection::create_ttf_from_memory(byte *buffer,
													 uint32_t width,
													 uint32_t height) {
	GL_CHECK;
	uint32_t texture_id_{0};
	//	uint32_t sampler_id_{0};

	glGenTextures(1, &texture_id_);
	glBindTexture(GL_TEXTURE_2D, texture_id_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
				 GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, buffer);
	GL_CHECK;
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GL_CHECK;

	//    glGenSamplers(1, &sampler_id_);
	//    glSamplerParameteri(sampler_id_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//    glSamplerParameteri(sampler_id_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//	GL_CHECK;
	glBindTexture(GL_TEXTURE_2D, 0);
	//    ASSERT(texture_id_ == sampler_id_);
	GL_CHECK;
	return texture_id_;
}

uint32_t Texture2DCollection::create_sampler() {
	uint32_t sampler;
	glGenSamplers(1, &sampler);
	return sampler;
}

void Texture2DCollection::destroy(uint32_t texture_id) {
	glDeleteTextures(1, &texture_id);
	//	glDeleteSamplers(1, &texture_id);
	GL_CHECK;
}

void Texture2DCollection::bind(uint32_t texture_id, uint32_t unit) {
	glActiveTexture(GL_TEXTURE0 + unit);
	//	glUniform1i(texture_id, unit);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	//    glBindSampler(unit, texture_id);
	GL_CHECK;
}

void Texture2DCollection::bind_sampler(uint32_t sampler_id, uint32_t unit) {
	glBindSampler(unit, sampler_id);
}

void Texture2DCollection::bind(const uint32_t *texture_ids, uint32_t size,
							   uint32_t start_unit) {
	ASSERT(texture_ids != nullptr);
    ASSERT(size > 0);
	const uint32_t size_ = size + start_unit;
	for (uint32_t i = start_unit; i < size_; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
		const uint32_t id = texture_ids[i];
		glBindTexture(GL_TEXTURE_2D, id);
		//        glBindSampler(i, id);
		GL_CHECK;
    }
}

void Texture2DCollection::unbind(uint32_t unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, 0);
	//    glBindSampler(unit,0);
	GL_CHECK;
}

void Texture2DCollection::unbind_sampler(uint32_t unit) {
	glBindSampler(unit, 0);
}

void Texture2DCollection::save(const char *filename, uint32_t texture_id) {
	GLint width, height, internalFormat;
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glGetTexLevelParameteriv(
		GL_TEXTURE_2D, 0, GL_TEXTURE_COMPONENTS,
		&internalFormat); // get internal format type of GL texture
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH,
							 &width); // get width of GL texture
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT,
							 &height); // get height of GL texture
	// GL_TEXTURE_COMPONENTS and GL_INTERNAL_FORMAT are the same.
	// just work with RGB8 and RGBA8
	GLint numBytes = 0;
	GLint numChanels = 0;
	int type = -1;
	switch (internalFormat) // determine what type GL texture has...
	{
	case GL_RGB:
		numBytes = width * height * 3;
		numChanels = 3;
		type = GL_UNSIGNED_BYTE;
		break;
	case GL_RGBA:
	case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
		numBytes = width * height * 4;
		numChanels = 4;
		type = GL_UNSIGNED_BYTE;
		break;
	case GL_RGB32F:
		numBytes = width * height * 3;
		numChanels = 3;
		type = GL_FLOAT;
		break;
	default: // unsupported type (or you can put some code to support more
			 // formats if you need)
		break;
	}

	if (numBytes) {
		unsigned char *pixels =
			(unsigned char *)malloc(numBytes); // allocate image data into RAM
		glGetTexImage(GL_TEXTURE_2D, 0, internalFormat, type, pixels);
		{
			SOIL_save_image(filename, SOIL_SAVE_TYPE_BMP, width, height,
							numChanels, pixels);
			printf("Save image to: %s - %s\n", filename, SOIL_last_result());
			// TODO with pixels
		}
		free(pixels); // when you don't need 'pixels' anymore clean a memory
					  // page to avoid memory leak.
	}
}

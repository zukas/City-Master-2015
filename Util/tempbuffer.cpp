#include "tempbuffer.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include "glvalidator.h"

TempBuffer::TempBuffer() {}

void TempBuffer::init(uint32_t width, uint32_t height, type_t type) {
	glGenFramebuffers(1, &m_frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);
	GL_CHECK;
	glGenTextures(1, &m_colour_buffer);
	glBindTexture(GL_TEXTURE_2D, m_colour_buffer);
	GL_CHECK;
	switch (type) {
	case RGBA:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
					 GL_FLOAT, NULL);
		break;
	case RGB:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
					 GL_FLOAT, NULL);
		break;
	case RGB16F:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB,
					 GL_FLOAT, NULL);
		break;
	default:
		break;
	}

	GL_CHECK;
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_colour_buffer,
						 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	GL_CHECK_FB;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TempBuffer::begin_write() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);
	GL_CHECK;
}

void TempBuffer::end_write()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GL_CHECK;
}

uint32_t TempBuffer::colour_buffer_id() const
{
	return m_colour_buffer;
}


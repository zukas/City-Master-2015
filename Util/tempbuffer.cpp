#include "tempbuffer.h"
#include "viewport.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include "glvalidator.h"

TempBuffer::TempBuffer() {}

void TempBuffer::init(uint32_t width, uint32_t height) {
	m_width = width;
	m_height = height;
	glGenFramebuffers(1, &m_frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);
	GL_CHECK;
	glGenTextures(1, &m_colour_buffer);
	glBindTexture(GL_TEXTURE_2D, m_colour_buffer);
	GL_CHECK;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT,
				 NULL);
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

void TempBuffer::end_write() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GL_CHECK;
}

void TempBuffer::copy_to_buffer(uint32_t buffer_id)
{

	float _viewport_width = Viewport::width();
	float _viewport_height = Viewport::height();
	GL_CHECK;
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frame_buffer);
	GL_CHECK;
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buffer_id);
	GL_CHECK;
	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, _viewport_width,
					  _viewport_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GL_CHECK;
}

uint32_t TempBuffer::colour_buffer_id() const { return m_colour_buffer; }

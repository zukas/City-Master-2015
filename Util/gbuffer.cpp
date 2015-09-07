#include "gbuffer.h"
#include "viewport.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <stdio.h>

GBuffer::~GBuffer() { clear(); }

void GBuffer::init() {
	clear();

	float _viewport_width = Viewport::width();
	float _viewport_height = Viewport::height();

	glGenFramebuffers(1, &m_frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);

	// - Position color buffer
	glGenTextures(1, &m_position_buffer);
	glBindTexture(GL_TEXTURE_2D, m_position_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _viewport_width, _viewport_height,
				 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
						   m_position_buffer, 0);
	// - Normal color buffer
	glGenTextures(1, &m_normal_buffer);
	glBindTexture(GL_TEXTURE_2D, m_normal_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _viewport_width, _viewport_height, 0,
				 GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
						   m_normal_buffer, 0);
	// - Color + Specular color buffer
	glGenTextures(1, &m_colour_buffer);
	glBindTexture(GL_TEXTURE_2D, m_colour_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _viewport_width, _viewport_height,
				 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
						   m_colour_buffer, 0);

	GLuint attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
							 GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, attachments);

	// - Depth buffer
	glGenRenderbuffers(1, &m_depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depth_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _viewport_width,
						  _viewport_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
							  GL_RENDERBUFFER, m_depth_buffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::clear() {}

void GBuffer::begin_geom_pass() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GBuffer::end_geom_pass() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void GBuffer::begin_render_pass() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_normal_buffer);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_position_buffer);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_colour_buffer);
}

void GBuffer::end_render_pass() {
	float _viewport_width = Viewport::width();
	float _viewport_height = Viewport::height();
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frame_buffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Write to default framebuffer
	glBlitFramebuffer(0, 0, _viewport_width, _viewport_height, 0, 0,
					  _viewport_width, _viewport_height, GL_DEPTH_BUFFER_BIT,
					  GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::blit_buffer(uint32_t id) {
	float _viewport_width = Viewport::width();
	float _viewport_height = Viewport::height();
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frame_buffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Write to default framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + id);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, _viewport_width, _viewport_height, 0, 0, _viewport_width,
					  _viewport_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

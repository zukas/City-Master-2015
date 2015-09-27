#include "gbuffer.h"
#include "viewport.h"
#include "glvalidator.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <stdio.h>

#define SAMPLE_COUNT 4

GBuffer::~GBuffer() { clear(); }

void GBuffer::init() {
    clear();
	GL_CHECK;
    float _viewport_width = Viewport::width();
    float _viewport_height = Viewport::height();

	glGenFramebuffers(1, &m_multisample.frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_multisample.frame_buffer);
	GL_CHECK;
    // - Position color buffer
	glGenTextures(1, &m_multisample.position_buffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_multisample.position_buffer);
	GL_CHECK;
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, SAMPLE_COUNT, GL_RGB16F,
							_viewport_width, _viewport_height, GL_TRUE);

	GL_CHECK;
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
						 m_multisample.position_buffer, 0);

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    // - Normal color buffer
	glGenTextures(1, &m_multisample.normal_buffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_multisample.normal_buffer);

	GL_CHECK;
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, SAMPLE_COUNT, GL_RGB16F,
							_viewport_width, _viewport_height, GL_TRUE);
	GL_CHECK;
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
						 m_multisample.normal_buffer, 0);
	GL_CHECK;
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    // - Color + Specular color buffer
	glGenTextures(1, &m_multisample.colour_buffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_multisample.colour_buffer);

	GL_CHECK;
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, SAMPLE_COUNT, GL_RGBA16F,
							_viewport_width, _viewport_height, GL_TRUE);
	GL_CHECK;
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
						 m_multisample.colour_buffer, 0);
	GL_CHECK;

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	glGenTextures(1, &m_multisample.depth_buffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_multisample.depth_buffer);

	GL_CHECK;
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, SAMPLE_COUNT,
							GL_DEPTH_COMPONENT24, _viewport_width,
							_viewport_height, GL_TRUE);
	GL_CHECK;
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
						 m_multisample.depth_buffer, 0);
	GL_CHECK;

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	GLuint attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
							 GL_COLOR_ATTACHMENT2};

	glDrawBuffers(3, attachments);

	GL_CHECK;

	GL_CHECK_FB;

	glGenFramebuffers(1, &m_final.frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_final.frame_buffer);
	GL_CHECK;
	// - Position color buffer
	glGenTextures(1, &m_final.position_buffer);
	glBindTexture(GL_TEXTURE_2D, m_final.position_buffer);
	GL_CHECK;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _viewport_width, _viewport_height, 0,
				 GL_RGB, GL_FLOAT, NULL);
	GL_CHECK;
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
						 m_final.position_buffer, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	// - Normal color buffer
	glGenTextures(1, &m_final.normal_buffer);
	glBindTexture(GL_TEXTURE_2D, m_final.normal_buffer);

	GL_CHECK;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _viewport_width, _viewport_height, 0,
				 GL_RGB, GL_FLOAT, NULL);

	GL_CHECK;
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
						 m_final.normal_buffer, 0);
	GL_CHECK;
	glBindTexture(GL_TEXTURE_2D, 0);

	// - Color + Specular color buffer
	glGenTextures(1, &m_final.colour_buffer);
	glBindTexture(GL_TEXTURE_2D, m_final.colour_buffer);

	GL_CHECK;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _viewport_width, _viewport_height,
				 0, GL_RGBA, GL_FLOAT, NULL);

	GL_CHECK;
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
						 m_final.colour_buffer, 0);
	GL_CHECK;

	glBindTexture(GL_TEXTURE_2D, 0);

	glDrawBuffers(3, attachments);

	GL_CHECK;

	GL_CHECK_FB;

	glGenSamplers(1, &m_sampler);

	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::clear() {}

void GBuffer::begin_geom_pass() {

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glBindFramebuffer(GL_FRAMEBUFFER, m_multisample.frame_buffer);
	GL_CHECK;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GL_CHECK;
}

void GBuffer::end_geom_pass() {

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	float _viewport_width = Viewport::width();
	float _viewport_height = Viewport::height();

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_multisample.frame_buffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_final.frame_buffer);
	GL_CHECK;
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, _viewport_width, _viewport_height, 0, 0,
					  _viewport_width, _viewport_height, GL_COLOR_BUFFER_BIT,
					  GL_NEAREST);
	GL_CHECK;

	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glDrawBuffer(GL_COLOR_ATTACHMENT1);
	glBlitFramebuffer(0, 0, _viewport_width, _viewport_height, 0, 0,
					  _viewport_width, _viewport_height, GL_COLOR_BUFFER_BIT,
					  GL_NEAREST);
	GL_CHECK;

	glReadBuffer(GL_COLOR_ATTACHMENT2);
	glDrawBuffer(GL_COLOR_ATTACHMENT2);
	glBlitFramebuffer(0, 0, _viewport_width, _viewport_height, 0, 0,
					  _viewport_width, _viewport_height, GL_COLOR_BUFFER_BIT,
					  GL_NEAREST);
	GL_CHECK;
	glBlitFramebuffer(0, 0, _viewport_width, _viewport_height, 0, 0,
					  _viewport_width, _viewport_height, GL_DEPTH_BUFFER_BIT,
					  GL_NEAREST);
	GL_CHECK;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::begin_render_pass() {

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glClear(GL_COLOR_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
	glBindSampler(0, m_sampler);
	glBindTexture(GL_TEXTURE_2D, m_final.position_buffer);
	GL_CHECK;
	glActiveTexture(GL_TEXTURE1);
	glBindSampler(1, m_sampler);
	glBindTexture(GL_TEXTURE_2D, m_final.normal_buffer);
	GL_CHECK;
	glActiveTexture(GL_TEXTURE2);
	glBindSampler(2, m_sampler);
	glBindTexture(GL_TEXTURE_2D, m_final.colour_buffer);
	GL_CHECK;
}

void GBuffer::end_render_pass() {

	glDisable(GL_BLEND);
	GL_CHECK;
	glActiveTexture(GL_TEXTURE0);
	glBindSampler(0, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	GL_CHECK;
	glActiveTexture(GL_TEXTURE1);
	glBindSampler(1, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	GL_CHECK;
	glActiveTexture(GL_TEXTURE2);
	glBindSampler(2, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	GL_CHECK;
}

uint32_t GBuffer::colour_buffer_id() { return m_final.colour_buffer; }

// glBindTexture(GL_TEXTURE_2D, m_depthTexture);
// glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, WindowWidth,
// WindowHeight, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
// glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
// GL_TEXTURE_2D, m_depthTexture, 0);

void GBuffer2::init(uint32_t width, uint32_t height, uint32_t samples) {
	clear();
	glGenFramebuffers(1, &m_frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);
	GL_CHECK;

	glGenTextures(3, &m_position_buffer);

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_position_buffer);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width,
							height, GL_TRUE);
	GL_CHECK;
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_normal_buffer);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width,
							height, GL_TRUE);
	GL_CHECK;
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_colour_buffer);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width,
							height, GL_TRUE);
	GL_CHECK;
	glGenRenderbuffers(1, &m_depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depth_buffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples,
									 GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
							  GL_RENDERBUFFER, m_depth_buffer);
	GL_CHECK;

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
						 m_position_buffer, 0);
	GL_CHECK;
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
						 m_normal_buffer, 0);
	GL_CHECK;
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
						 m_colour_buffer, 0);
	GL_CHECK;
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
						 m_depth_buffer, 0);
	GL_CHECK;
	GL_CHECK_FB;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer2::clear() {}

void GBuffer2::start_frame() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frame_buffer);
}

void GBuffer2::geom_pass() {

	GLuint attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
							 GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, attachments);
}

void GBuffer2::stencli_pass() { glDrawBuffer(GL_NONE); }

void GBuffer2::light_pass() {}

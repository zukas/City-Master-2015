#include "gbuffer.h"
#include "viewport.h"
#include "glvalidator.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <stdio.h>

GBuffer::~GBuffer() { clear(); }

void GBuffer::init(uint32_t width, uint32_t height, uint32_t samples) {
    clear();
    GL_CHECK;

    m_width = width;
    m_height = height;

    glGenFramebuffers(1, &m_multisample.frame_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_multisample.frame_buffer);
    GL_CHECK;
    // - Position color buffer
    glGenTextures(1, &m_multisample.position_buffer);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_multisample.position_buffer);
    GL_CHECK;
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB16F,
                            width, height, GL_TRUE);

    GL_CHECK;
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                         m_multisample.position_buffer, 0);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    // - Normal color buffer
    glGenTextures(1, &m_multisample.normal_buffer);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_multisample.normal_buffer);

    GL_CHECK;
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB16F,
                            width, height, GL_TRUE);
    GL_CHECK;
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
                         m_multisample.normal_buffer, 0);
    GL_CHECK;
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    // - Color + Specular color buffer
    glGenTextures(1, &m_multisample.colour_buffer);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_multisample.colour_buffer);

    GL_CHECK;
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB16F,
                            width, height, GL_TRUE);
    GL_CHECK;
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
                         m_multisample.colour_buffer, 0);
    GL_CHECK;

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    glGenTextures(1, &m_multisample.depth_buffer);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_multisample.depth_buffer);

    GL_CHECK;
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples,
                            GL_DEPTH_COMPONENT24, width, height, GL_TRUE);
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB,
                 GL_FLOAT, NULL);
    GL_CHECK;
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                         m_final.position_buffer, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    // - Normal color buffer
    glGenTextures(1, &m_final.normal_buffer);
    glBindTexture(GL_TEXTURE_2D, m_final.normal_buffer);

    GL_CHECK;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB,
                 GL_FLOAT, NULL);

    GL_CHECK;
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
                         m_final.normal_buffer, 0);
    GL_CHECK;
    glBindTexture(GL_TEXTURE_2D, 0);

    // - Color + Specular color buffer
    glGenTextures(1, &m_final.colour_buffer);
    glBindTexture(GL_TEXTURE_2D, m_final.colour_buffer);

    GL_CHECK;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB,
                 GL_FLOAT, NULL);

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
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, _viewport_width,
                      _viewport_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    GL_CHECK;

    glReadBuffer(GL_COLOR_ATTACHMENT1);
    glDrawBuffer(GL_COLOR_ATTACHMENT1);
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, _viewport_width,
                      _viewport_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    GL_CHECK;

    glReadBuffer(GL_COLOR_ATTACHMENT2);
    glDrawBuffer(GL_COLOR_ATTACHMENT2);
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, _viewport_width,
                      _viewport_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    GL_CHECK;
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, _viewport_width,
                      _viewport_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
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

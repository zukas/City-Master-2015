#include "smaa.h"
#include "viewport.h"

#include <GL/glew.h>
#include <GL/gl.h>

#define AREA_WIDTH 160u
#define AREA_HEIGHT 560u
#define SEARCH_WIDTH 66u
#define SEARCH_HEIGHT 33u





uint32_t create_enge_detection_program(const char *shader_source, uint32_t vireport_width, uint32_t viewport_heaight)
{

}

uint32_t create_blend_weight_program(const char *shader_source, uint32_t vireport_width, uint32_t viewport_heaight)
{

}

uint32_t create_neighbor_blend_program(const char *shader_source, uint32_t vireport_width, uint32_t viewport_heaight)
{

}


void SMAA::init(const char *shader_source, byte *area, byte *search) {

    glGenTextures(2, m_data_buffers);

    glBindTexture(GL_TEXTURE_2D, m_data_buffers[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, AREA_WIDTH, AREA_HEIGHT, 0, GL_RG,
                 GL_UNSIGNED_BYTE, area);

    glBindTexture(GL_TEXTURE_2D, m_data_buffers[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, SEARCH_WIDTH, SEARCH_HEIGHT, 0,
                 GL_RED, GL_UNSIGNED_BYTE, search);

    glBindTexture(GL_TEXTURE_2D, 0);

    uint32_t _viewport_width = Viewport::width();
    uint32_t _viewport_height = Viewport::height();

    glGenTextures(3, m_texture_buffers);

    glBindTexture(GL_TEXTURE_2D, m_texture_buffers[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _viewport_width, _viewport_height,
                 0, GL_RGBA, GL_FLOAT, 0);

    glBindTexture(GL_TEXTURE_2D, m_texture_buffers[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _viewport_width, _viewport_height,
                 0, GL_RGBA, GL_FLOAT, 0);

    glBindTexture(GL_TEXTURE_2D, m_texture_buffers[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _viewport_width, _viewport_height,
                 0, GL_RGBA, GL_FLOAT, 0);

    GLenum modes[] = {GL_COLOR_ATTACHMENT0};

    glGenFramebuffers(3, m_frame_buffers);

    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers[0]);
    glDrawBuffers(1, modes);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           m_texture_buffers[0], 0);

    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers[1]);
    glDrawBuffers(1, modes);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           m_texture_buffers[1], 0);

    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers[2]);
    glDrawBuffers(1, modes);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           m_texture_buffers[2], 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void SMAA::run(uint32_t src_texture_buffer, uint32_t dest_frame_buffer) {}

void SMAA::enge_detection_pass() {}

void SMAA::blend_weight_pass() {}

void SMAA::neighbor_blend_pass() {}

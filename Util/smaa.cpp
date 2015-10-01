#include "smaa.h"
#include "viewport.h"
#include "Common/str_hash.h"
#include "mesh3dcollection.h"
#include "program.h"
#include "profiler.h"
#include "glvalidator.h"

#include <GL/glew.h>
#include <GL/gl.h>

#define AREA_WIDTH 160u
#define AREA_HEIGHT 560u
#define SEARCH_WIDTH 66u
#define SEARCH_HEIGHT 33u

#define NL "\n"

SMAA::edge_detection_program_t
create_edge_detection_program(const char *shader_source) {

    SMAA::edge_detection_program_t prog;

    const char *_vertex[3]{
        "#version 330 core" NL "#define SMAA_INCLUDE_VS 1" NL
        "#define SMAA_INCLUDE_PS 0" NL "#define SMAA_GLSL_3 1" NL
        "#define SMAA_PRESET_ULTRA 1" NL "uniform vec4 SMAA_RT_METRICS;" NL,
        shader_source,
        "layout (location = 0) in vec3 inPosition;" NL
        "layout (location = 1) in vec2 inUV;" NL "out vec2 UV;" NL
        "out vec4 offset[3];" NL "void main() {" NL
        "gl_Position = vec4(inPosition, 1.0);" NL "UV = inUV;" NL
        "SMAAEdgeDetectionVS(UV, offset);" NL "}" NL};

    const char *_fragment[3]{
        "#version 330 core" NL "#define SMAA_INCLUDE_PS 1" NL
        "#define SMAA_INCLUDE_VS 0" NL "#define SMAA_GLSL_3 1" NL
        "#define SMAA_PRESET_ULTRA 1" NL "uniform sampler2D colour_texture;" NL
        "uniform vec4 SMAA_RT_METRICS;" NL,
        shader_source,
        "in vec2 UV;" NL "in vec4 offset[3];" NL "out vec2 edges;" NL
        "void main() {" NL
		"edges = SMAALumaEdgeDetectionPS(UV, offset, colour_texture);" NL
        "}" NL};

    prog.program_id =
        ProgramCompiler::compileProgram(_vertex, 3, nullptr, 0, _fragment, 3);
    GL_CHECK;
	uint32_t uniforms[1];
	ProgramCompiler::resolveUniforms(prog.program_id, uniforms, 1);
    prog.rm_matrix_id =
		Uniforms::getUniformId(uniforms, 1, "SMAA_RT_METRICS"_h);
    GL_CHECK;
    glUseProgram(prog.program_id);
    GL_CHECK;
    Uniforms::setUniform(
		Uniforms::getUniformId(uniforms, 1, "colour_texture"_h), 0);
    GL_CHECK;
    glUseProgram(0);

    return prog;
}

SMAA::blend_weight_program_t
create_blend_weight_program(const char *shader_source) {
    SMAA::blend_weight_program_t prog;

    const char *_vertex[3]{
        "#version 330 core" NL "#define SMAA_INCLUDE_VS 1" NL
        "#define SMAA_INCLUDE_PS 0" NL "#define SMAA_GLSL_3 1" NL
        "#define SMAA_PRESET_ULTRA 1" NL "uniform vec4 SMAA_RT_METRICS;" NL,
        shader_source,
        "layout (location = 0) in vec3 inPosition;" NL
        "layout (location = 1) in vec2 inUV;" NL "out vec2 UV;" NL
        "out vec2 PIX;" NL "out vec4 offset[3];" NL "void main() {" NL
        "gl_Position = vec4(inPosition, 1.0);" NL "UV = inUV;" NL
        "SMAABlendingWeightCalculationVS(UV, PIX, offset);" NL "}" NL};

    const char *_fragment[3]{
        "#version 330 core" NL "#define SMAA_INCLUDE_PS 1" NL
        "#define SMAA_INCLUDE_VS 0" NL "#define SMAA_GLSL_3 1" NL
        "#define SMAA_PRESET_ULTRA 1" NL "uniform sampler2D edge_texture;" NL
        "uniform sampler2D area_texture;" NL
        "uniform sampler2D search_texture;" NL
        "uniform vec4 SMAA_RT_METRICS;" NL,
        shader_source,
        "in vec2 UV;" NL "in vec2 PIX;" NL "in vec4 offset[3];" NL
        "out vec4 weights;" NL "void main() {" NL
        "weights = SMAABlendingWeightCalculationPS(UV, PIX, offset, "
        "edge_texture, area_texture, search_texture, vec4(1, 1, 1, 0));" NL
        "}" NL};

    prog.program_id =
        ProgramCompiler::compileProgram(_vertex, 3, nullptr, 0, _fragment, 3);
    GL_CHECK;
    uint32_t uniforms[4];
    ProgramCompiler::resolveUniforms(prog.program_id, uniforms, 4);
    prog.rm_matrix_id =
        Uniforms::getUniformId(uniforms, 4, "SMAA_RT_METRICS"_h);

    glUseProgram(prog.program_id);
    GL_CHECK;
    uint32_t edge_texture_id =
        Uniforms::getUniformId(uniforms, 4, "edge_texture"_h);
    uint32_t area_texture_id =
        Uniforms::getUniformId(uniforms, 4, "area_texture"_h);
    uint32_t search_texture_id =
        Uniforms::getUniformId(uniforms, 4, "search_texture"_h);

    Uniforms::setUniform(edge_texture_id, 0);
    Uniforms::setUniform(area_texture_id, 1);
    Uniforms::setUniform(search_texture_id, 2);
    GL_CHECK;
    glUseProgram(0);
    return prog;
}

SMAA::neighbor_blend_program_t
create_neighbor_blend_program(const char *shader_source) {
    SMAA::neighbor_blend_program_t prog;

    const char *_vertex[3]{
        "#version 330 core" NL "#define SMAA_INCLUDE_VS 1" NL
        "#define SMAA_INCLUDE_PS 0" NL "#define SMAA_GLSL_3 1" NL
        "#define SMAA_PRESET_ULTRA 1" NL "uniform vec4 SMAA_RT_METRICS;" NL,
        shader_source,
        "layout (location = 0) in vec3 inPosition;" NL
        "layout (location = 1) in vec2 inUV;" NL "out vec2 UV;" NL
        "out vec4 offset;" NL "void main() {" NL
        "gl_Position = vec4(inPosition, 1.0);" NL "UV = inUV;" NL
        "SMAANeighborhoodBlendingVS(UV, offset);" NL "}" NL};

    const char *_fragment[3]{
        "#version 330 core" NL "#define SMAA_INCLUDE_PS 1" NL
        "#define SMAA_INCLUDE_VS 0" NL "#define SMAA_GLSL_3 1" NL
        "#define SMAA_PRESET_ULTRA 1" NL "uniform sampler2D colour_texture;" NL
        "uniform sampler2D blend_texture;" NL
        "uniform vec4 SMAA_RT_METRICS;" NL,
        shader_source,
        "in vec2 UV;" NL "in vec4 offset;" NL "out vec4 colour;" NL
        "void main() {" NL "colour = SMAANeighborhoodBlendingPS(UV, offset, "
        "colour_texture, blend_texture);" NL "}" NL};

    prog.program_id =
        ProgramCompiler::compileProgram(_vertex, 3, nullptr, 0, _fragment, 3);
    GL_CHECK;
    uint32_t uniforms[3];
    ProgramCompiler::resolveUniforms(prog.program_id, uniforms, 3);
    prog.rm_matrix_id =
        Uniforms::getUniformId(uniforms, 3, "SMAA_RT_METRICS"_h);

    glUseProgram(prog.program_id);
    GL_CHECK;
    uint32_t colour_texture_id =
        Uniforms::getUniformId(uniforms, 3, "colour_texture"_h);
    uint32_t blend_texture_id =
        Uniforms::getUniformId(uniforms, 3, "blend_texture"_h);

    Uniforms::setUniform(colour_texture_id, 0);
    Uniforms::setUniform(blend_texture_id, 1);
    GL_CHECK;
    glUseProgram(0);

    return prog;
}

void SMAA::init(const char *shader_source, byte *area, byte *search) {

    GL_CHECK;
    clear();
    uv_3d_vertex vertexes[4]{{{-1.f, 1.f, 0.f}, {0.f, 1.f}},
                             {{-1.f, -1.f, 0.f}, {0.f, 0.f}},
                             {{1.f, -1.f, 0.f}, {1.f, 0.f}},
                             {{1.f, 1.f, 0.f}, {1.f, 1.f}}};
    uint32_t indexes[6]{0, 1, 2, 2, 3, 0};
    GL_CHECK;
    m_mesh_id = Mesh3DCollection::create(vertexes, 4, indexes, 6);
    GL_CHECK;
    glGenTextures(2, m_data_buffers);
    GL_CHECK;
    glBindTexture(GL_TEXTURE_2D, m_data_buffers[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, AREA_WIDTH, AREA_HEIGHT, 0, GL_RG,
                 GL_UNSIGNED_BYTE, area);
    GL_CHECK;
    glBindTexture(GL_TEXTURE_2D, m_data_buffers[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, SEARCH_WIDTH, SEARCH_HEIGHT, 0,
                 GL_RED, GL_UNSIGNED_BYTE, search);
    GL_CHECK;
    glBindTexture(GL_TEXTURE_2D, 0);

    uint32_t _viewport_width = Viewport::width();
    uint32_t _viewport_height = Viewport::height();

    glGenTextures(2, m_texture_buffers);

    glBindTexture(GL_TEXTURE_2D, m_texture_buffers[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _viewport_width,
                 _viewport_height, 0, GL_RGBA, GL_FLOAT, 0);
    GL_CHECK;
    glBindTexture(GL_TEXTURE_2D, m_texture_buffers[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _viewport_width,
                 _viewport_height, 0, GL_RGBA, GL_FLOAT, 0);
    GL_CHECK;
    GLenum modes[] = {GL_COLOR_ATTACHMENT0};

    glGenFramebuffers(2, m_frame_buffers);

    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers[0]);
    glDrawBuffers(1, modes);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           m_texture_buffers[0], 0);
    GL_CHECK;
    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers[1]);
    glDrawBuffers(1, modes);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           m_texture_buffers[1], 0);
    GL_CHECK;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_edge_detection_program = create_edge_detection_program(shader_source);
    m_blend_weight_program = create_blend_weight_program(shader_source);
    m_neighbor_blend_program = create_neighbor_blend_program(shader_source);

	glGenSamplers(1, &m_sampler);

	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void SMAA::clear() {}

void SMAA::run(uint32_t src_texture_buffer, uint32_t dest_frame_buffer) const {

	glBindSampler(0, m_sampler);
    edge_detection_pass(src_texture_buffer);
    run_pass();
    GL_CHECK;
	blend_weight_pass();
	run_pass();
	GL_CHECK;
	neighbor_blend_pass(src_texture_buffer, dest_frame_buffer);
	run_pass();

	glBindSampler(0, 0);
	glBindSampler(1, 0);
}

void SMAA::edge_detection_pass(uint32_t colour_texture_id) const {

	glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers[0]);
    glClear(GL_COLOR_BUFFER_BIT);
    GL_CHECK;
    glUseProgram(m_edge_detection_program.program_id);
    GL_CHECK;

//    float _viewport_width = Viewport::width();
//    float _viewport_height = Viewport::height();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colour_texture_id);

//    GL_CHECK;
//    Uniforms::setUniform(m_blend_weight_program.rm_matrix_id,
//                         glm::vec4(1.f / _viewport_width,
//                                   1.f / _viewport_height, _viewport_width,
//                                   _viewport_height));

    GL_CHECK;
}

void SMAA::blend_weight_pass() const {

    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers[1]);
    glClear(GL_COLOR_BUFFER_BIT);
    GL_CHECK;
    glUseProgram(m_blend_weight_program.program_id);
    GL_CHECK;
    float _viewport_width = Viewport::width();
    float _viewport_height = Viewport::height();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture_buffers[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_data_buffers[0]);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_data_buffers[1]);
    GL_CHECK;
    Uniforms::setUniform(m_blend_weight_program.rm_matrix_id,
                         glm::vec4(1.f / _viewport_width,
                                   1.f / _viewport_height, _viewport_width,
                                   _viewport_height));
    GL_CHECK;
}

void SMAA::neighbor_blend_pass(uint32_t colour_texture_id,
                               uint32_t dst_framebuffer_id) const {

    glBindFramebuffer(GL_FRAMEBUFFER, dst_framebuffer_id);
    glClear(GL_COLOR_BUFFER_BIT);
    GL_CHECK;

    glUseProgram(m_neighbor_blend_program.program_id);
    GL_CHECK;
    float _viewport_width = Viewport::width();
    float _viewport_height = Viewport::height();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colour_texture_id);
    GL_CHECK;
	glBindSampler(1, m_sampler);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture_buffers[1]);
    GL_CHECK;

    Uniforms::setUniform(m_neighbor_blend_program.rm_matrix_id,
                         glm::vec4(1.f / _viewport_width,
                                   1.f / _viewport_height, _viewport_width,
                                   _viewport_height));
    GL_CHECK;
}

void SMAA::run_pass() const {
    Mesh3DCollection::render_geometry(m_mesh_id, 6);
    GL_CHECK;
}

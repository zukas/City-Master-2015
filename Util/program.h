#ifndef PROGRAM_H
#define PROGRAM_H

#include "types.h"
#include <glm/glm.hpp>

enum ShaderType { VERTEX, GEOMETRY, FRAGMENT };

struct ShaderSource {
	ShaderType type;
	const char *source;
};

namespace ProgramCompiler {
uint32_t compileProgram(const ShaderSource *shaders, uint32_t size);
uint32_t
compileProgram(const char **vertex_shaders, uint32_t vertex_shader_count,
			   const char **geometry_shaders, uint32_t geometry_shader_count,
			   const char **fragment_shaders, uint32_t fragment_shader_count);

int32_t compile_shaders(uint32_t shader_id, const char **shader_sources,
					 uint32_t shader_count);

void resolveUniforms(uint32_t programId, uint32_t *uniforms, uint32_t size);
}

namespace Uniforms {
uint32_t getUniformId(const uint32_t *uniforms, uint32_t size,
					  uint32_t uniformHash);
void setUniform(uint32_t uniformId, int32_t value);
void setUniform(uint32_t uniformId, uint32_t value);
void setUniform(uint32_t uniformId, float_t value);
void setUniform(uint32_t uniformId, const glm::vec2 &value);
void setUniform(uint32_t uniformId, const glm::vec3 &value);
void setUniform(uint32_t uniformId, const glm::vec4 &value);
void setUniform(uint32_t uniformId, const glm::mat3 &value);
void setUniform(uint32_t uniformId, const glm::mat4 &value);
}

#endif // PROGRAM_H

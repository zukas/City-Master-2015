#ifndef PROGRAM_H
#define PROGRAM_H

#include "types.h"
#include <glm/glm.hpp>

enum ShaderType { VERTEX, GEOMETRY, FRAGMENT };

struct Shader {
	ShaderType type;
	const char *source;
};

namespace ProgramCompiler {
uint32_t compileProgram(const Shader *shaders, uint32_t size);
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

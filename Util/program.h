#ifndef PROGRAM_H
#define PROGRAM_H

#include "types.h"
#include <glm/glm.hpp>

enum ShaderType { VERTEX, GEOMETRY, FRAGMENT };

struct ShaderSource {
    ShaderType type;
    const char *source;
};

template <uint32_t __S> class UniformBuffer {
  private:
    uint32_t m_buffer[__S];

  public:
    UniformBuffer(uint32_t program_id);
    uint32_t operator[] (uint32_t hash);
};

namespace Program {

uint32_t compile(const ShaderSource *shaders, uint32_t size);
uint32_t compile(const char **vertex_shaders, uint32_t vertex_shader_count,
                 const char **geometry_shaders, uint32_t geometry_shader_count,
                 const char **fragment_shaders, uint32_t fragment_shader_count);
int32_t compile(uint32_t shader_id, const char **shader_sources,
                uint32_t shader_count);

void resolve_uniforms(uint32_t program_id, uint32_t *uniforms, uint32_t size);

uint32_t get_uniform_id(const uint32_t *uniforms, uint32_t size,
                        uint32_t uniform_hash);

void set_uniform(uint32_t uniformId, int32_t value);
void set_uniform(uint32_t uniformId, uint32_t value);
void set_uniform(uint32_t uniformId, float_t value);
void set_uniform(uint32_t uniformId, const glm::vec2 &value);
void set_uniform(uint32_t uniformId, const glm::vec3 &value);
void set_uniform(uint32_t uniformId, const glm::vec4 &value);
void set_uniform(uint32_t uniformId, const glm::mat3 &value);
void set_uniform(uint32_t uniformId, const glm::mat4 &value);
}

template <uint32_t __S>
UniformBuffer<__S >::UniformBuffer(uint32_t program_id)
{
    Program::resolve_uniforms(program_id, m_buffer, __S);
}

template <uint32_t __S>
uint32_t UniformBuffer<__S >::operator[](uint32_t hash)
{
    return Program::get_uniform_id(m_buffer, __S, hash);
}

#endif // PROGRAM_H

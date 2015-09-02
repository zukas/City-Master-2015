#include "program.h"
#include "Common/str_hash.h"

#include <stdio.h>

#include <GL/glew.h>
#include <GL/gl.h>

uint32_t ProgramCompiler::compileProgram(const Shader *shaders, uint32_t size) {
    ASSERT(size > 0);

    uint32_t programId = glCreateProgram();

    GLint status{GL_FALSE};
    for (uint32_t i = 0; i < size; ++i) {

        GLuint shaderId{0};

        switch (shaders[i].type) {
        case VERTEX:
            shaderId = glCreateShader(GL_VERTEX_SHADER);
            break;
        case GEOMETRY:
            shaderId = glCreateShader(GL_GEOMETRY_SHADER);
            break;
        case FRAGMENT:
            shaderId = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        default:
            break;
        }

        const char *source = shaders[i].source;

        glShaderSource(shaderId, 1, &source, nullptr);
        glCompileShader(shaderId);
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);

        if (!status) {
            int logLength{0};
            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
            if (logLength > 0) {
                char error[2048]{0};
                glGetShaderInfoLog(shaderId, logLength, nullptr, error);
                printf("Failed to compile shader\n%s\nCompile error: %s\n",
                       shaders[i].source, error);
                exit(1);
            }
        } else {
            glAttachShader(programId, shaderId);
            glDeleteShader(shaderId);
        }
    }

    glLinkProgram(programId);
    glGetProgramiv(programId, GL_LINK_STATUS, &status);
    if (!status) {
        int logLength{0};
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            char error[2048]{0};
            glGetProgramInfoLog(programId, logLength, nullptr, error);
            printf("Program link error: %s\n", error);
            exit(1);
        }
    }

    return programId;
}

void ProgramCompiler::resolveUniforms(uint32_t programId, uint32_t *uniforms,
                                      uint32_t size) {
    GLint count;
    glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &count);
    ASSERT((uint32_t)count == size);

    char name[256];
    GLsizei length;

    for (uint32_t i = 0; i < size; ++i) {
        GLint _size;
        GLenum _type;
        glGetActiveUniform(programId, i, 256, &length, &_size, &_type, name);
        uint32_t _crc32 = crc32(name, length);
        uniforms[i] = _crc32;

        uint32_t loc_ = glGetUniformLocation(programId, name);
        printf("name: %s, location: %u, i: %u\n", name, loc_, i);
    }
}

uint32_t Uniforms::getUniformId(const uint32_t *uniforms, uint32_t size,
                                uint32_t uniformHash) {
    uint32_t id = 0;
    for (uint32_t i = 0; id == 0 && i < size; ++i) {
        if (uniforms[i] == uniformHash) {
            id = i;
        }
    }
    return id;
}

void Uniforms::setUniform(uint32_t uniformId, int32_t value) {
    glUniform1i(uniformId, value);
}

void Uniforms::setUniform(uint32_t uniformId, uint32_t value) {
    glUniform1ui(uniformId, value);
}

void Uniforms::setUniform(uint32_t uniformId, float_t value) {
    glUniform1f(uniformId, value);
}

void Uniforms::setUniform(uint32_t uniformId, const glm::vec2 &value) {
    glUniform2fv(uniformId, 1, &value[0]);
}

void Uniforms::setUniform(uint32_t uniformId, const glm::vec3 &value) {
    glUniform3fv(uniformId, 1, &value[0]);
}

void Uniforms::setUniform(uint32_t uniformId, const glm::vec4 &value) {
    glUniform4fv(uniformId, 1, &value[0]);
}

void Uniforms::setUniform(uint32_t uniformId, const glm::mat3 &value) {
    glUniformMatrix3fv(uniformId, 1, GL_FALSE, &value[0][0]);
}

void Uniforms::setUniform(uint32_t uniformId, const glm::mat4 &value) {
    glUniformMatrix4fv(uniformId, 1, GL_FALSE, &value[0][0]);
}

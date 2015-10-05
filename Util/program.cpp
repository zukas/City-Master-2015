#include "program.h"
#include "Common/str_hash.h"

#include <stdio.h>

#include <GL/glew.h>
#include <GL/gl.h>

uint32_t Program::compile(const ShaderSource *shaders,
										 uint32_t size) {
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

uint32_t Program::compile(const char **vertex_shaders,
										 uint32_t vertex_shader_count,
										 const char **geometry_shaders,
										 uint32_t geometry_shader_count,
										 const char **fragment_shaders,
										 uint32_t fragment_shader_count) {

	uint32_t programId = glCreateProgram();

	if (vertex_shader_count > 0) {
		uint32_t shader_id = glCreateShader(GL_VERTEX_SHADER);
		if (!compile(shader_id, vertex_shaders, vertex_shader_count)) {
			exit(1);
		} else {
			glAttachShader(programId, shader_id);
			glDeleteShader(shader_id);
		}
	}

	if (geometry_shader_count > 0) {
		uint32_t shader_id = glCreateShader(GL_GEOMETRY_SHADER);
		if (!compile(shader_id, geometry_shaders,
							 geometry_shader_count)) {
			exit(1);
		} else {
			glAttachShader(programId, shader_id);
			glDeleteShader(shader_id);
		}
	}

	if (fragment_shader_count > 0) {
		uint32_t shader_id = glCreateShader(GL_FRAGMENT_SHADER);
		if (!compile(shader_id, fragment_shaders,
							 fragment_shader_count)) {
			exit(1);
		} else {
			glAttachShader(programId, shader_id);
			glDeleteShader(shader_id);
		}
	}

	GLint status{GL_FALSE};
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

int32_t Program::compile(uint32_t shader_id,
										 const char **shader_sources,
										 uint32_t shader_count) {
	GLint status{GL_FALSE};
	glShaderSource(shader_id, shader_count, shader_sources, nullptr);
	glCompileShader(shader_id);
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
	if (!status) {
		int logLength{0};
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			char error[4096]{0};
			glGetShaderInfoLog(shader_id, 4096, nullptr, error);
			printf("Failed to compile shader - error: %s\n", error);
		}
	}
	return status;
}

void Program::resolve_uniforms(uint32_t program_id, uint32_t *uniforms,
                                      uint32_t size) {
    GLint count;
    glGetProgramiv(program_id, GL_ACTIVE_UNIFORMS, &count);
	if ((uint32_t)count != size) {
		printf("Incorrect uniform count, expected: %u, but requested: %u\n",
			   count, size);
//		ASSERT((uint32_t)count == size);
	}

    char name[256];
    GLsizei length;

    for (uint32_t i = 0; i < size; ++i) {
        GLint _size;
        GLenum _type;
        glGetActiveUniform(program_id, i, 256, &length, &_size, &_type, name);
        uint32_t _loc = glGetUniformLocation(program_id, name);
		ASSERT(_loc == i);
        uint32_t _crc32 = crc32(name, length);
		uniforms[_loc] = _crc32;

        printf("name: %s, location: %u, i: %u (%u)\n", name, _loc, i, _crc32);
    }
}

uint32_t Program::get_uniform_id(const uint32_t *uniforms, uint32_t size,
                                uint32_t uniform_hash) {
    uint32_t id = size;
    for (uint32_t i = 0; id == size && i < size; ++i) {
        if (uniforms[i] == uniform_hash) {
            id = i;
        }
    }
    return id;
}

void Program::set_uniform(uint32_t uniformId, int32_t value) {
    glUniform1i(uniformId, value);
}

void Program::set_uniform(uint32_t uniformId, uint32_t value) {
    glUniform1ui(uniformId, value);
}

void Program::set_uniform(uint32_t uniformId, float_t value) {
    glUniform1f(uniformId, value);
}

void Program::set_uniform(uint32_t uniformId, const glm::vec2 &value) {
    glUniform2fv(uniformId, 1, &value[0]);
}

void Program::set_uniform(uint32_t uniformId, const glm::vec3 &value) {
    glUniform3fv(uniformId, 1, &value[0]);
}

void Program::set_uniform(uint32_t uniformId, const glm::vec4 &value) {
    glUniform4fv(uniformId, 1, &value[0]);
}

void Program::set_uniform(uint32_t uniformId, const glm::mat3 &value) {
    glUniformMatrix3fv(uniformId, 1, GL_FALSE, &value[0][0]);
}

void Program::set_uniform(uint32_t uniformId, const glm::mat4 &value) {
    glUniformMatrix4fv(uniformId, 1, GL_FALSE, &value[0][0]);
}

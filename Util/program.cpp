#include "program.h"
#include "utils.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <fstream>

RefCount Program::g_counter;

void Program::cleanUp() {
	if (m_programID != 0 && g_counter - m_programID == 0) {
        glDeleteProgram(m_programID);
        m_programID = 0;
    }
}

Program::Program() : m_programID(0) {}

Program::Program(const std::string &vertex, const std::string &fragment,
				 ProgramType type)
    : m_programID(0), m_type(type) {
    createShader(vertex, VERTEX);
    createShader(fragment, FRAGMENT);
    createProgram();
    linkProgram();
    resolveUniforms();
}

Program::Program(Program &&other)
	: m_programID(other.m_programID), m_type(other.m_type), m_ids(other.m_ids) {
    other.m_programID = 0;
}

Program::Program(const Program &other)
	: m_programID(other.m_programID), m_type(other.m_type), m_ids(other.m_ids) {
	g_counter + m_programID;
}

Program::~Program() {
    cleanUp();

	for (auto id : m_shaders) {
        glDeleteShader(id);
    }
    m_shaders.clear();
}

void Program::use() {
    glUseProgram(m_programID);
}

ProgramType Program::type() const { return m_type; }

void Program::type(ProgramType t) { m_type = t; }

void Program::createShader(const Shader &shader) {
	if (shader.source) {
		GLuint shaderID{0};
		switch (shader.type) {
		case VERTEX:
			shaderID = glCreateShader(GL_VERTEX_SHADER);
			break;
		case GEOMETRY:
			shaderID = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		case FRAGMENT:
			shaderID = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		default:
			break;
		}

		GLint status{GL_FALSE};

		const char *data = shader.source;

		glShaderSource(shaderID, 1, &data, nullptr);
		glCompileShader(shaderID);
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

		if (status) {
			m_shaders.push_back(shaderID);
		} else {
			int logLength{0};
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
			if (logLength > 0) {
				char *error = new char[logLength + 1];
				glGetShaderInfoLog(shaderID, logLength, nullptr, error);
				printf("Failed to compile shader\n%s\nCompile error: %s\n",
					   shader.source, error);
				exit(1);
			}
		}
	}
}

void Program::createShader(const std::string &file, ShaderType t) {
    std::string code;
    std::ifstream handle(file);
	if (handle.is_open()) {
        std::string line = "";
		while (getline(handle, line))
            code += "\n" + line;
        handle.close();
    }

	if (!code.empty()) {
		createShader({t, code.c_str()});
    }
}

void Program::createProgram() {
    cleanUp();
    m_programID = glCreateProgram();
	g_counter + m_programID;
	for (auto id : m_shaders) {
        glAttachShader(m_programID, id);
    }
}

void Program::linkProgram() {

	GLint status{GL_FALSE};
    glLinkProgram(m_programID);
    glGetProgramiv(m_programID, GL_LINK_STATUS, &status);
	if (!status) {
		int logLength{0};
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
            char *error = new char[logLength + 1];
            glGetProgramInfoLog(m_programID, logLength, nullptr, error);
            printf("Program link error: %s\n", error);
            exit(1);
        }
        cleanUp();
    }
	for (auto id : m_shaders) {
        glDeleteShader(id);
    }
    m_shaders.clear();
}

void Program::resolveUniforms() {
	m_ids.glsl_model_matrix =
		glGetUniformLocation(m_programID, glsl_model_matrix);
	m_ids.glsl_view_matrix =
		glGetUniformLocation(m_programID, glsl_view_matrix);
	m_ids.glsl_projection_matrix =
		glGetUniformLocation(m_programID, glsl_projection_matrix);
	m_ids.glsl_camera_position =
		glGetUniformLocation(m_programID, glsl_camera_position);
    m_ids.glsl_quad[0] = glGetUniformLocation(m_programID, glsl_quad[0]);
    m_ids.glsl_quad[1] = glGetUniformLocation(m_programID, glsl_quad[1]);

	m_ids.glsl_light_position =
		glGetUniformLocation(m_programID, glsl_light_position);
	m_ids.glsl_light_strength =
		glGetUniformLocation(m_programID, glsl_light_strength);

    m_ids.glsl_sampler[0] = glGetUniformLocation(m_programID, glsl_sampler[0]);
    m_ids.glsl_sampler[1] = glGetUniformLocation(m_programID, glsl_sampler[1]);
    m_ids.glsl_sampler[2] = glGetUniformLocation(m_programID, glsl_sampler[2]);
    m_ids.glsl_sampler[3] = glGetUniformLocation(m_programID, glsl_sampler[3]);
    m_ids.glsl_sampler[4] = glGetUniformLocation(m_programID, glsl_sampler[4]);

	m_ids.glsl_texture_count =
		glGetUniformLocation(m_programID, glsl_texture_count);
    m_ids.glsl_object_id = glGetUniformLocation(m_programID, glsl_object_id);
	m_ids.glsl_object_selected =
		glGetUniformLocation(m_programID, glsl_object_selected);
    m_ids.glsl_colour = glGetUniformLocation(m_programID, glsl_colour);
}

GLID Program::program() { return m_programID; }

void Program::setModelMatrix(const glm::mat4 &mat) {
	glProgramUniformMatrix4fv(m_programID, m_ids.glsl_model_matrix, 1, GL_FALSE,
							  &mat[0][0]);
}

void Program::setViewMatrix(const glm::mat4 &mat) {
	glProgramUniformMatrix4fv(m_programID, m_ids.glsl_view_matrix, 1, GL_FALSE,
							  &mat[0][0]);
}

void Program::setProjectionMatrix(const glm::mat4 &mat) {
	glProgramUniformMatrix4fv(m_programID, m_ids.glsl_projection_matrix, 1,
							  GL_FALSE, &mat[0][0]);
}

void Program::setCameraPosition(const glm::vec3 &vec) {
    glProgramUniform3fv(m_programID, m_ids.glsl_camera_position, 1, &vec[0]);
}

void Program::setQuads(const glm::vec3 *quads) {
    glProgramUniform3fv(m_programID, m_ids.glsl_quad[0], 1, &quads[0][0]);
    glProgramUniform3fv(m_programID, m_ids.glsl_quad[1], 1, &quads[1][0]);
}

void Program::setLigthPosition(const glm::vec3 &vec) {
    glProgramUniform3fv(m_programID, m_ids.glsl_light_position, 1, &vec[0]);
}

void Program::setLightStrength(float value) {
    glProgramUniform1f(m_programID, m_ids.glsl_light_strength, value);
}

void Program::setSampers(glint value) {
	for (int i = 0; i < value; ++i) {
		glProgramUniform1i(m_programID, m_ids.glsl_sampler[i], i);
	}

	glProgramUniform1i(m_programID, m_ids.glsl_texture_count, value);
}

void Program::setObjectID(const glm::vec4 &vec) {
    glProgramUniform4fv(m_programID, m_ids.glsl_object_id, 1, &vec[0]);
}

void Program::setSelected(bool value) {
    glProgramUniform1i(m_programID, m_ids.glsl_object_selected, value);
}

void Program::setColour(const glm::vec4 &vec) {
    glProgramUniform4fv(m_programID, m_ids.glsl_colour, 1, &vec[0]);
}

rect Program::getViewport() const { return {}; }

Program &Program::operator=(Program &&other) {
    cleanUp();
    m_programID = other.m_programID;
    m_type = other.m_type;
    m_ids = other.m_ids;
    other.m_programID = 0;
    return *this;
}

Program &Program::operator=(const Program &other) {
    cleanUp();
    m_programID = other.m_programID;
    m_type = other.m_type;
    m_ids = other.m_ids;
	g_counter + m_programID;
    return *this;
}

// UniformManager::UniformManager(Program *prog) : m_prog(prog) {}

// void UniformManager::setUniform(const char *name, bool value) {
//	glint id = glGetUniformLocation(m_prog->program(), name);
//	glProgramUniform1i(m_prog->program(), id, value);
//}

// void UniformManager::setUniform(const char *name, GLID value) {
//	glint id = glGetUniformLocation(m_prog->program(), name);
//	glProgramUniform1ui(m_prog->program(), id, value);
//}

// void UniformManager::setUniform(const char *name, glint value) {
//	glint id = glGetUniformLocation(m_prog->program(), name);
//	glProgramUniform1i(m_prog->program(), id, value);
//}

// void UniformManager::setUniform(const char *name, float value) {
//	glint id = glGetUniformLocation(m_prog->program(), name);
//	glProgramUniform1f(m_prog->program(), id, value);
//}

// void UniformManager::setUniform(const char *name, const glm::vec2 &vec) {
//	glint id = glGetUniformLocation(m_prog->program(), name);
//	glProgramUniform2fv(m_prog->program(), id, 1, &vec[0]);
//}

// void UniformManager::setUniform(const char *name, const glm::vec3 &vec) {
//	glint id = glGetUniformLocation(m_prog->program(), name);
//	glProgramUniform3fv(m_prog->program(), id, 1, &vec[0]);
//}

// void UniformManager::setUniform(const char *name, const glm::vec4 &vec) {
//	glint id = glGetUniformLocation(m_prog->program(), name);
//	glProgramUniform4fv(m_prog->program(), id, 1, &vec[0]);
//}

// void UniformManager::setUniform(const char *name, const glm::mat4 &mat) {
//	glint id = glGetUniformLocation(m_prog->program(), name);
//	glProgramUniformMatrix4fv(m_prog->program(), id, 1, GL_FALSE,
//&mat[0][0]);
//}

// void UniformManager::setUniform(glint id, bool value) {
//	glProgramUniform1ui(m_prog->program(), id, value);
//}

// void UniformManager::setUniform(glint id, GLID value) {
//	glProgramUniform1ui(m_prog->program(), id, value);
//}

// void UniformManager::setUniform(glint id, glint value) {
//	glProgramUniform1i(m_prog->program(), id, value);
//}

// void UniformManager::setUniform(glint id, float value) {
//	glProgramUniform1f(m_prog->program(), id, value);
//}

// void UniformManager::setUniform(glint id, const glm::vec2 &vec) {
//	glProgramUniform2fv(m_prog->program(), id, 1, &vec[0]);
//}

// void UniformManager::setUniform(glint id, const glm::vec3 &vec) {
//	glProgramUniform3fv(m_prog->program(), id, 1, &vec[0]);
//}

// void UniformManager::setUniform(glint id, const glm::vec4 &vec) {
//	glProgramUniform4fv(m_prog->program(), id, 1, &vec[0]);
//}

// void UniformManager::setUniform(glint id, const glm::mat4 &mat) {
//	glProgramUniformMatrix4fv(m_prog->program(), id, 1, GL_FALSE,
//&mat[0][0]);
//}

// glint UniformManager::resolveUniform(const char *name) const {
//	return glGetUniformLocation(m_prog->program(), name);
//}

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
		uint32_t crc32 = crypto::crc32(name, length);
		uniforms[i] = crc32;

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

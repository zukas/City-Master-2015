#ifndef PROGRAM_H
#define PROGRAM_H

#include "types.h"
#include "utils.h"

#include "refcount.h"
#include <glm/glm.hpp>

enum ProgramType { Normal, Selection };

enum ShaderType { VERTEX, GEOMETRY, FRAGMENT };

constexpr const char *glsl_model_matrix{"modelMatrix"};
constexpr const char *glsl_view_matrix{"viewMatrix"};
constexpr const char *glsl_projection_matrix{"projectionMatrix"};
constexpr const char *glsl_camera_position{"cameraPosition"};
constexpr const char *glsl_quad[2]{"vQuad1", "vQuad2"};
constexpr const char *glsl_light_position{"lightPosition"};
constexpr const char *glsl_light_strength{"lightStrength"};
constexpr const char *glsl_transform_vector{"transform"};
constexpr const char *glsl_sampler[5]{
	"tsampler[0]", "tsampler[1]", "tsampler[2]", "tsampler[3]", "tsampler[4]"};
constexpr const char *glsl_texture_count{"tcount"};
constexpr const char *glsl_mesh_type{"mtype"};
constexpr const char *glsl_object_id{"objectID"};
constexpr const char *glsl_object_selected{"objectSelected"};
constexpr const char *glsl_colour{"colour"};

struct Shader {
	ShaderType type;
	const char *source;
};

constexpr Shader operator"" _vs(const char *source, std::size_t) {
	return {VERTEX, source};
}

constexpr Shader operator"" _gs(const char *source, std::size_t) {
	return {GEOMETRY, source};
}

constexpr Shader operator"" _fs(const char *source, std::size_t) {
	return {FRAGMENT, source};
}

#define GLSL(value, type) "#version 330\n" #value type
#define VS(value) GLSL(value, ""_vs)
#define GS(value) GLSL(value, ""_gs)
#define FS(value) GLSL(value, ""_fs)

class Program;

// class UniformManager {
//  private:
//	Program *m_prog;

//  public:
//	UniformManager(Program *prog);

//	void setUniform(const char *name, bool value);
//	void setUniform(const char *name, GLID value);
//	void setUniform(const char *name, glint value);
//	void setUniform(const char *name, float value);
//	void setUniform(const char *name, const glm::vec2 &vec);
//	void setUniform(const char *name, const glm::vec3 &vec);
//	void setUniform(const char *name, const glm::vec4 &vec);
//	void setUniform(const char *name, const glm::mat4 &mat);

//	void setUniform(glint id, bool value);
//	void setUniform(glint id, GLID value);
//	void setUniform(glint id, glint value);
//	void setUniform(glint id, float value);
//	void setUniform(glint id, const glm::vec2 &vec);
//	void setUniform(glint id, const glm::vec3 &vec);
//	void setUniform(glint id, const glm::vec4 &vec);
//	void setUniform(glint id, const glm::mat4 &mat);

//	glint resolveUniform(const char *name) const;
//};

class Program {
  private:
	uint32_t m_programID;
	std::vector<uint32_t> m_shaders;
	ProgramType m_type{Normal};
//	ViewportManager m_viewport;
	struct {
		glint glsl_model_matrix{0};
		glint glsl_view_matrix{0};
		glint glsl_projection_matrix{0};
		glint glsl_camera_position{0};
		glint glsl_quad[2]{0};
		glint glsl_light_position{0};
		glint glsl_light_strength{0};
		glint glsl_sampler[5]{0};
		glint glsl_texture_count{0};
		glint glsl_object_id{0};
		glint glsl_object_selected{0};
		glint glsl_colour{0};
	} m_ids;

	static RefCount g_counter;

  private:
	void cleanUp();

  public:
	Program();
	Program(const std::string &vertex, const std::string &fragment,
			ProgramType type = Normal);
	Program(Program &&other);
	Program(const Program &other);
	~Program();

	void use();
	ProgramType type() const;
	void type(ProgramType t);
	void createShader(const Shader &shader);
	void createShader(const std::string &file, ShaderType t);
	void createProgram();
	void linkProgram();
	void resolveUniforms();
	GLID program();
	rect getViewport() const;

	void setModelMatrix(const glm::mat4 &mat);
	void setViewMatrix(const glm::mat4 &mat);
	void setProjectionMatrix(const glm::mat4 &mat);
	void setCameraPosition(const glm::vec3 &vec);
	void setQuads(const glm::vec3 *quads);
	void setLigthPosition(const glm::vec3 &vec);
	void setLightStrength(float value);
	void setSampers(glint value);
	void setObjectID(const glm::vec4 &vec);
	void setSelected(bool value);
	void setColour(const glm::vec4 &vec);

	Program &operator=(Program &&other);
	Program &operator=(const Program &other);
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

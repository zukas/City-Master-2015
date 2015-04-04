#ifndef PROGRAM_H
#define PROGRAM_H

#include "types.h"
#include "refcount.h"
#include <glm/glm.hpp>

enum ProgramType
{
    Normal, Selection
};

enum ShaderType
{
    VERTEX, GEOMETRY, FRAGMENT
};


struct Viewport
{
        int x { 0 };
        int y { 0 };
        int w { 0 };
        int h { 0 };
};

constexpr const char *glsl_model_matrix { "modelMatrix" };
constexpr const char *glsl_view_matrix { "viewMatrix" };
constexpr const char *glsl_projection_matrix { "projectionMatrix" };
constexpr const char *glsl_camera_position { "cameraPosition" };
constexpr const char *glsl_light_position { "lightPosition" };
constexpr const char *glsl_light_strength { "lightStrength" };
constexpr const char *glsl_transform_vector { "transform" };
constexpr const char *glsl_sampler[5] { "tsampler[0]", "tsampler[1]", "tsampler[2]", "tsampler[3]", "tsampler[4]" };
constexpr const char *glsl_texture_count { "tcount" };
constexpr const char *glsl_mesh_type { "mtype" };
constexpr const char *glsl_object_id { "objectID" };
constexpr const char *glsl_object_selected { "objectSelected" };
constexpr const char *glsl_colour { "colour" };

class Program
{
    private:
        GLID m_programID;
        std::vector<GLID > m_shaders;
        ProgramType m_type;
        Viewport m_viewport;
        struct
        {
            GLSID glsl_model_matrix { 0 };
            GLSID glsl_view_matrix { 0 };
            GLSID glsl_projection_matrix { 0 };
            GLSID glsl_camera_position { 0 };
            GLSID glsl_light_position { 0 };
            GLSID glsl_light_strength { 0 };
            GLSID glsl_sampler[5] { 0 };
            GLSID glsl_texture_count { 0 };
            GLSID glsl_object_id { 0 };
            GLSID glsl_object_selected { 0 };
            GLSID glsl_colour { 0 };
        } m_ids;

        static RefCount g_counter;

    private:
        void cleanUp();

    public:
        Program();
        Program(const std::string &vertex, const std::string &fragment, ProgramType type = Normal);
        Program(Program &&other);
        Program(const Program &other);
        ~Program();

        void use();
        ProgramType type() const;
        void type(ProgramType t);
        void createShader(const std::string &file, ShaderType t);
        void createProgram();
        void linkProgram();
        void resolveUniforms();
        GLID program();
        Viewport getViewport() const;


        void setModelMatrix(const glm::mat4 &mat);
        void setViewMatrix(const glm::mat4 &mat);
        void setProjectionMatrix(const glm::mat4 &mat);
        void setCameraPosition(const glm::vec3 &vec);
        void setLigthPosition(const glm::vec3 &vec);
        void setLightStrength(float value);
        void setSampers(GLID value);
        void setObjectID(const glm::vec4 &vec);
        void setSelected(bool value);
        void setColour(const glm::vec4 &vec);


        void setUniform(const std::string &name, bool value);
        void setUniform(const std::string &name, GLID value);
        void setUniform(const std::string &name, GLSID value);
        void setUniform(const std::string &name, float value);
        void setUniform(const std::string &name, const glm::vec2 &vec);
        void setUniform(const std::string &name, const glm::vec3 &vec);
        void setUniform(const std::string &name, const glm::vec4 &vec);
        void setUniform(const std::string &name, const glm::mat4 &mat);
        Program &operator = (Program &&other);
        Program &operator = (const Program &other);

};

#endif // PROGRAM_H

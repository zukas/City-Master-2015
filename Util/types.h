#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>

typedef unsigned GLID;
typedef int GLSID;

constexpr float PI { 3.14 };

struct Error
{
        std::string message;
        enum
        {
            NONE,

            FAILED_INIT_GLFW,
            FAILED_INIT_GLEW,

            FAILED_COMPILE_FRAGMENT_SHADER,
            FAILED_COMPILE_VERTEX_SHADER,
            FAILED_LINK_PROGRAM,

            NOT_SUPPORTED_GL3_3,

            FILE_NOT_FOUND
        } code { NONE };
};



constexpr const char *glsl_model_matrix { "modelMatrix" };
constexpr const char *glsl_view_matrix { "viewMatrix" };
constexpr const char *glsl_projection_matrix { "projectionMatrix" };
constexpr const char *glsl_light_position { "lightPosition" };
constexpr const char *glsl_light_strength { "lightStrength" };
constexpr const char *glsl_transform_vector { "transform" };
constexpr const char *glsl_sampler[5] { "tsampler[0]", "tsampler[1]", "tsampler[2]", "tsampler[3]", "tsampler[4]" };
constexpr const char *glsl_texture_count { "tcount" };
constexpr const char *glsl_mesh_type { "mtype" };
constexpr const char *glsl_object_id { "objectID" };
constexpr const char *glsl_object_selected { "objectSelected" };



#endif // TYPES_H

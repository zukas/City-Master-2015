#ifndef UNIFORMS_H
#define UNIFORMS_H

#include "types.h"

constexpr GLID glsl_model_mat4 { 1 };
constexpr GLID glsl_view_mat4 { 2 };
constexpr GLID glsl_projection_mat4 { 3 };
constexpr GLID glsl_model_view_mat4 { 4 };
constexpr GLID glsl_camera_vec3 { 5 };
constexpr GLID glsl_light_source_vec4 { 6 };
constexpr GLID glsl_sampler_int[2] { 7, 8 };
constexpr GLID glsl_texture_count_int { 9 };
constexpr GLID glsl_object_id_vec4 { 10 };

#endif // UNIFORMS_H


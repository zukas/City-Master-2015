#include "resourcemanager.h"
#include <cstdlib>
#include "res_particles_render_frag_glsl.h"
#include "res_skybox_frag_glsl.h"
#include "res_basic_frag_glsl.h"
#include "res_particles_render_geom_glsl.h"
#include "res_particles_update_frag_glsl.h"
#include "res_object_frag_glsl.h"
#include "res_text_frag_glsl.h"
#include "res_text_vert_glsl.h"
#include "res_basic_vert_glsl.h"
#include "res_particles_render_vert_glsl.h"
#include "res_particles_update_vert_glsl.h"
#include "res_object_select_frag_glsl.h"
#include "res_particles_update_geom_glsl.h"
#include "res_object_vert_glsl.h"
#include "res_object_select_vert_glsl.h"
#include "res_skybox_vert_glsl.h"
malloc_function __g_malloc_func { malloc };
void set_malloc_function(malloc_function func) { __g_malloc_func = func; }
resource_buffer load_buffer_resource(buffer_resource_t type) {
switch(type) {
default: return resource_buffer { nullptr, 0 };}
}
const char *load_litteral_resource(literal_resource_t type) {
switch(type) {
case literal_resource_t::RES_PARTICLES_RENDER_FRAG_GLSL: return get_res_particles_render_frag_glsl();
case literal_resource_t::RES_SKYBOX_FRAG_GLSL: return get_res_skybox_frag_glsl();
case literal_resource_t::RES_BASIC_FRAG_GLSL: return get_res_basic_frag_glsl();
case literal_resource_t::RES_PARTICLES_RENDER_GEOM_GLSL: return get_res_particles_render_geom_glsl();
case literal_resource_t::RES_PARTICLES_UPDATE_FRAG_GLSL: return get_res_particles_update_frag_glsl();
case literal_resource_t::RES_OBJECT_FRAG_GLSL: return get_res_object_frag_glsl();
case literal_resource_t::RES_TEXT_FRAG_GLSL: return get_res_text_frag_glsl();
case literal_resource_t::RES_TEXT_VERT_GLSL: return get_res_text_vert_glsl();
case literal_resource_t::RES_BASIC_VERT_GLSL: return get_res_basic_vert_glsl();
case literal_resource_t::RES_PARTICLES_RENDER_VERT_GLSL: return get_res_particles_render_vert_glsl();
case literal_resource_t::RES_PARTICLES_UPDATE_VERT_GLSL: return get_res_particles_update_vert_glsl();
case literal_resource_t::RES_OBJECT_SELECT_FRAG_GLSL: return get_res_object_select_frag_glsl();
case literal_resource_t::RES_PARTICLES_UPDATE_GEOM_GLSL: return get_res_particles_update_geom_glsl();
case literal_resource_t::RES_OBJECT_VERT_GLSL: return get_res_object_vert_glsl();
case literal_resource_t::RES_OBJECT_SELECT_VERT_GLSL: return get_res_object_select_vert_glsl();
case literal_resource_t::RES_SKYBOX_VERT_GLSL: return get_res_skybox_vert_glsl();
default: return "";}
}

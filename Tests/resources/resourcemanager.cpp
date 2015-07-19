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
#include "res_freesans_ttf.h"
#include "res_ganymede_dds.h"
#include "res_uranus_dds.h"
#include "res_enceladus_dds.h"
#include "res_sun_dds.h"
#include "res_saturn_dds.h"
#include "res_neptune_dds.h"
#include "res_rhea_dds.h"
#include "res_mercury_dds.h"
#include "res_mercurymap_dds.h"
#include "res_titan_dds.h"
#include "res_earth_dds.h"
#include "res_mimas_dds.h"
#include "res_moon_dds.h"
#include "res_venusmap_dds.h"
#include "res_europa_dds.h"
#include "res_dione_dds.h"
#include "res_jupiter_dds.h"
#include "res_mars_dds.h"
#include "res_iapetus_dds.h"
#include "res_io_dds.h"
#include "res_tethys_dds.h"
#include "res_callisto_dds.h"
malloc_function __g_malloc_func { malloc };
void set_malloc_function(malloc_function func) { __g_malloc_func = func; }
resource_buffer load_buffer_resource(buffer_resource_t type) {
switch(type) {
case buffer_resource_t::RES_FREESANS_TTF: return resource_buffer { get_res_freesans_ttf(__g_malloc_func(res_freesans_ttf_size)), res_freesans_ttf_size };
case buffer_resource_t::RES_GANYMEDE_DDS: return resource_buffer { get_res_ganymede_dds(__g_malloc_func(res_ganymede_dds_size)), res_ganymede_dds_size };
case buffer_resource_t::RES_URANUS_DDS: return resource_buffer { get_res_uranus_dds(__g_malloc_func(res_uranus_dds_size)), res_uranus_dds_size };
case buffer_resource_t::RES_ENCELADUS_DDS: return resource_buffer { get_res_enceladus_dds(__g_malloc_func(res_enceladus_dds_size)), res_enceladus_dds_size };
case buffer_resource_t::RES_SUN_DDS: return resource_buffer { get_res_sun_dds(__g_malloc_func(res_sun_dds_size)), res_sun_dds_size };
case buffer_resource_t::RES_SATURN_DDS: return resource_buffer { get_res_saturn_dds(__g_malloc_func(res_saturn_dds_size)), res_saturn_dds_size };
case buffer_resource_t::RES_NEPTUNE_DDS: return resource_buffer { get_res_neptune_dds(__g_malloc_func(res_neptune_dds_size)), res_neptune_dds_size };
case buffer_resource_t::RES_RHEA_DDS: return resource_buffer { get_res_rhea_dds(__g_malloc_func(res_rhea_dds_size)), res_rhea_dds_size };
case buffer_resource_t::RES_MERCURY_DDS: return resource_buffer { get_res_mercury_dds(__g_malloc_func(res_mercury_dds_size)), res_mercury_dds_size };
case buffer_resource_t::RES_MERCURYMAP_DDS: return resource_buffer { get_res_mercurymap_dds(__g_malloc_func(res_mercurymap_dds_size)), res_mercurymap_dds_size };
case buffer_resource_t::RES_TITAN_DDS: return resource_buffer { get_res_titan_dds(__g_malloc_func(res_titan_dds_size)), res_titan_dds_size };
case buffer_resource_t::RES_EARTH_DDS: return resource_buffer { get_res_earth_dds(__g_malloc_func(res_earth_dds_size)), res_earth_dds_size };
case buffer_resource_t::RES_MIMAS_DDS: return resource_buffer { get_res_mimas_dds(__g_malloc_func(res_mimas_dds_size)), res_mimas_dds_size };
case buffer_resource_t::RES_MOON_DDS: return resource_buffer { get_res_moon_dds(__g_malloc_func(res_moon_dds_size)), res_moon_dds_size };
case buffer_resource_t::RES_VENUSMAP_DDS: return resource_buffer { get_res_venusmap_dds(__g_malloc_func(res_venusmap_dds_size)), res_venusmap_dds_size };
case buffer_resource_t::RES_EUROPA_DDS: return resource_buffer { get_res_europa_dds(__g_malloc_func(res_europa_dds_size)), res_europa_dds_size };
case buffer_resource_t::RES_DIONE_DDS: return resource_buffer { get_res_dione_dds(__g_malloc_func(res_dione_dds_size)), res_dione_dds_size };
case buffer_resource_t::RES_JUPITER_DDS: return resource_buffer { get_res_jupiter_dds(__g_malloc_func(res_jupiter_dds_size)), res_jupiter_dds_size };
case buffer_resource_t::RES_MARS_DDS: return resource_buffer { get_res_mars_dds(__g_malloc_func(res_mars_dds_size)), res_mars_dds_size };
case buffer_resource_t::RES_IAPETUS_DDS: return resource_buffer { get_res_iapetus_dds(__g_malloc_func(res_iapetus_dds_size)), res_iapetus_dds_size };
case buffer_resource_t::RES_IO_DDS: return resource_buffer { get_res_io_dds(__g_malloc_func(res_io_dds_size)), res_io_dds_size };
case buffer_resource_t::RES_TETHYS_DDS: return resource_buffer { get_res_tethys_dds(__g_malloc_func(res_tethys_dds_size)), res_tethys_dds_size };
case buffer_resource_t::RES_CALLISTO_DDS: return resource_buffer { get_res_callisto_dds(__g_malloc_func(res_callisto_dds_size)), res_callisto_dds_size };
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

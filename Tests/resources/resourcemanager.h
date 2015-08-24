#ifndef RESOURCES_H
#define RESOURCES_H
#include <cstddef>
typedef void* (*malloc_function)(size_t);
void set_malloc_function(malloc_function);
void init_resources();
void deinit_resources();
struct resource {
unsigned char *buffer;
size_t size;
};
resource get_res_freesans_ttf ();
resource get_res_venus_dds ();
resource get_res_ganymede_dds ();
resource get_res_uranus_dds ();
resource get_res_enceladus_dds ();
resource get_res_sun_dds ();
resource get_res_saturn_dds ();
resource get_res_neptune_dds ();
resource get_res_rhea_dds ();
resource get_res_mercury_dds ();
resource get_res_titan_dds ();
resource get_res_earth_dds ();
resource get_res_mimas_dds ();
resource get_res_moon_dds ();
resource get_res_europa_dds ();
resource get_res_dione_dds ();
resource get_res_jupiter_dds ();
resource get_res_mars_dds ();
resource get_res_iapetus_dds ();
resource get_res_io_dds ();
resource get_res_tethys_dds ();
resource get_res_callisto_dds ();

const char *get_res_particles_render_frag_glsl ();
const char *get_res_skybox_frag_glsl ();
const char *get_res_basic_frag_glsl ();
const char *get_res_particles_render_geom_glsl ();
const char *get_res_particles_update_frag_glsl ();
const char *get_res_object_frag_glsl ();
const char *get_res_text_frag_glsl ();
const char *get_res_text_vert_glsl ();
const char *get_res_basic_vert_glsl ();
const char *get_res_particles_render_vert_glsl ();
const char *get_res_particles_update_vert_glsl ();
const char *get_res_object_select_frag_glsl ();
const char *get_res_particles_update_geom_glsl ();
const char *get_res_object_vert_glsl ();
const char *get_res_object_select_vert_glsl ();
const char *get_res_skybox_vert_glsl ();

#endif //RESOURCES_H
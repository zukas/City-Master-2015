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
resource get_res_freesans_ttf (void*);
size_t get_res_freesans_ttf_size();
resource get_res_venus_dds (void*);
size_t get_res_venus_dds_size();
resource get_res_ganymede_dds (void*);
size_t get_res_ganymede_dds_size();
resource get_res_uranus_dds (void*);
size_t get_res_uranus_dds_size();
resource get_res_enceladus_dds (void*);
size_t get_res_enceladus_dds_size();
resource get_res_sun_dds (void*);
size_t get_res_sun_dds_size();
resource get_res_saturn_dds (void*);
size_t get_res_saturn_dds_size();
resource get_res_neptune_dds (void*);
size_t get_res_neptune_dds_size();
resource get_res_rhea_dds (void*);
size_t get_res_rhea_dds_size();
resource get_res_mercury_dds (void*);
size_t get_res_mercury_dds_size();
resource get_res_titan_dds (void*);
size_t get_res_titan_dds_size();
resource get_res_earth_dds (void*);
size_t get_res_earth_dds_size();
resource get_res_mimas_dds (void*);
size_t get_res_mimas_dds_size();
resource get_res_moon_dds (void*);
size_t get_res_moon_dds_size();
resource get_res_europa_dds (void*);
size_t get_res_europa_dds_size();
resource get_res_dione_dds (void*);
size_t get_res_dione_dds_size();
resource get_res_jupiter_dds (void*);
size_t get_res_jupiter_dds_size();
resource get_res_mars_dds (void*);
size_t get_res_mars_dds_size();
resource get_res_iapetus_dds (void*);
size_t get_res_iapetus_dds_size();
resource get_res_io_dds (void*);
size_t get_res_io_dds_size();
resource get_res_tethys_dds (void*);
size_t get_res_tethys_dds_size();
resource get_res_callisto_dds (void*);
size_t get_res_callisto_dds_size();

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
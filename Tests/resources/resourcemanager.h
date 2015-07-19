#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <cstddef>
typedef void* (*malloc_function)(size_t);
void set_malloc_function(malloc_function);
enum class buffer_resource_t {
};
enum class literal_resource_t {
RES_PARTICLES_RENDER_FRAG_GLSL,
RES_SKYBOX_FRAG_GLSL,
RES_BASIC_FRAG_GLSL,
RES_PARTICLES_RENDER_GEOM_GLSL,
RES_PARTICLES_UPDATE_FRAG_GLSL,
RES_OBJECT_FRAG_GLSL,
RES_TEXT_FRAG_GLSL,
RES_TEXT_VERT_GLSL,
RES_BASIC_VERT_GLSL,
RES_PARTICLES_RENDER_VERT_GLSL,
RES_PARTICLES_UPDATE_VERT_GLSL,
RES_OBJECT_SELECT_FRAG_GLSL,
RES_PARTICLES_UPDATE_GEOM_GLSL,
RES_OBJECT_VERT_GLSL,
RES_OBJECT_SELECT_VERT_GLSL,
RES_SKYBOX_VERT_GLSL
};
struct resource_buffer {
unsigned char *buffer;
size_t size;
};
resource_buffer load_buffer_resource(buffer_resource_t);
const char *load_litteral_resource(literal_resource_t);
#endif //RESOURCEMANAGER_H
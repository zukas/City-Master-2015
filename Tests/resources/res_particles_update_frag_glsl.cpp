#include "res_particles_update_frag_glsl.h"
const char* get_res_particles_update_frag_glsl() { return "#version 330\n\nout vec4 vColor;\n\nvoid main()\n{\n    vColor = vec4(1.0, 1.0, 1.0, 1.0);\n}\n"; }

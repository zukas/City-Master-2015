#include "res_basic_frag_glsl.h"
const char* get_res_basic_frag_glsl() { return "#version 330\n\nin vec3 colour;\nout vec4 outColour;\n\nvoid main()\n{\n\n    outColour = vec4(colour, 0.1);\n}\n"; }

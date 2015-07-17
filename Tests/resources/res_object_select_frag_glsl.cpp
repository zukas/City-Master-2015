#include "res_object_select_frag_glsl.h"
const char* get_res_object_select_frag_glsl() { return "#version 330\n\nuniform vec4 objectID;\n\nout vec4 colour;\n\nvoid main()\n{\n    colour = objectID;\n}\n"; }

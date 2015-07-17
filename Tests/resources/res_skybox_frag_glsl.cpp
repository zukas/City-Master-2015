#include "res_skybox_frag_glsl.h"
const char* get_res_skybox_frag_glsl() { return "#version 330\n\nuniform samplerCube tsampler[1];\nsmooth in vec4 UV;\nout vec4 outColour;\n\nvoid main()\n{\n    outColour = texture(tsampler[0], UV);\n}\n"; }

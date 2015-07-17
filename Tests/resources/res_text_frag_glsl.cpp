#include "res_text_frag_glsl.h"
const char* get_res_text_frag_glsl() { return "#version 330\n\nin vec2 texCoord;\nout vec4 outColour;\n\nuniform sampler2D tsampler[1];\nuniform vec4 colour;\n\nvoid main()\n{\n        vec4 textColour = texture2D(tsampler[0], texCoord);\n        outColour = vec4(textColour.r, textColour.r, textColour.r, textColour.r) * colour;\n}\n"; }

#include "res_text_vert_glsl.h"
const char* get_res_text_vert_glsl() { return "#version 330\n\nuniform mat4 modelMatrix;\nuniform mat4 projectionMatrix;\n\nlayout (location = 0) in vec2 inPosition;\nlayout (location = 1) in vec2 inCoord;\n\nout vec2 texCoord;\n\nvoid main()\n{\n        gl_Position = projectionMatrix*modelMatrix*vec4(inPosition, 0.0, 1.0);\n        texCoord = inCoord;\n}\n"; }

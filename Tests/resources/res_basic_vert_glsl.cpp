#include "res_basic_vert_glsl.h"
const char* get_res_basic_vert_glsl() { return "#version 330\n\nlayout(location = 0) in vec3 inPosition;\nlayout(location = 1) in vec3 inColour;\n\nuniform mat4 viewMatrix;\nuniform mat4 projectionMatrix;\nuniform mat4 modelMatrix;\n\nout vec3 colour;\n\nvoid main()\n{\n    colour = inColour;\n    mat4 MVP = (projectionMatrix * viewMatrix * modelMatrix);\n    gl_Position =  MVP * vec4(inPosition, 1.0);\n}\n\n"; }
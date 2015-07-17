#include "res_particles_render_geom_glsl.h"
const char* get_res_particles_render_geom_glsl() { return "#version 330\n\nuniform mat4 viewMatrix;\nuniform mat4 projectionMatrix;\n\nuniform vec3 vQuad1, vQuad2;\n\nlayout(points) in;\nlayout(triangle_strip) out;\nlayout(max_vertices = 4) out;\n\nin vec3 vColorPass[1];\nin float fLifeTimePass[1];\nin float fSizePass[1];\nin float iTypePass[1];\n\nsmooth out vec2 vTexCoord;\nsmooth out vec4 vColorPart;\n\nvoid main()\n{\n        vec3 vPosOld = gl_in[0].gl_Position.xyz;\n        float fSize = fSizePass[0];\n        mat4 mVP = projectionMatrix* viewMatrix;\n\n//        float fdiv = fSize * (vQuad1+vQuad2).x / 2;\n        float val = fLifeTimePass[0];// / 500.0;\n        vColorPart = vec4(vColorPass[0], 1.0);\n\n        vec3 vPos = vPosOld + ((-vQuad1-vQuad2) * fSize);\n        vTexCoord = vec2(0.0, 0.0);\n        gl_Position = mVP * vec4(vPos, 1.0);\n        EmitVertex();\n\n        vPos = vPosOld + ((-vQuad1+vQuad2) * fSize);\n        vTexCoord = vec2(0.0, 1.0);\n        gl_Position = mVP * vec4(vPos, 1.0);\n        EmitVertex();\n\n        vPos = vPosOld + ((vQuad1-vQuad2) * fSize);\n        vTexCoord = vec2(1.0, 0.0);\n        gl_Position = mVP * vec4(vPos, 1.0);\n        EmitVertex();\n\n        vPos = vPosOld + ((vQuad1+vQuad2) * fSize);\n        vTexCoord = vec2(1.0, 1.0);\n        gl_Position = mVP * vec4(vPos, 1.0);\n        EmitVertex();\n\n        EndPrimitive();\n}\n"; }

#include "res_particles_render_frag_glsl.h"
const char* get_res_particles_render_frag_glsl() { return "#version 330\n\nuniform sampler2D tsampler[1];\nuniform int tcount;\n\nsmooth in vec2 vTexCoord;\nsmooth in vec4 vColorPart;\n//in vec4 gl_FragCoord;\n\nout vec4 FragColor;\n\nvoid main()\n{\n    vec2 uv = vTexCoord.xy;\n//    uv.y *= -1.0;\n    vec3 vTexColor = texture2D(tsampler[0], uv).xyz;\n    float alpha = 0.0;\n    for(float i = 0.001; i < 1.0; i+=i)\n    {\n        if(vTexColor.r > i && vTexColor.g > i && vTexColor.b > i)\n        {\n            alpha = i;\n        }\n    }\n    if(alpha < 0.05)\n    {\n        discard;\n    }\n    else\n    {\n        alpha -= gl_FragCoord.z * 0.01;\n        FragColor = vec4(vColorPart.xyz, vColorPart.w * alpha);\n    }\n}\n"; }

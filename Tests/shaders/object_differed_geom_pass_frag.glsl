#version 330


layout (location = 0) out vec3 outPos;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec4 outColour;

in vec3 position;
in vec3 normal;
in vec2 UV;
uniform sampler2D texture_diffuse;

void main()
{
    outPos = position;
    outNormal = normal;
    outColour = texture2D(texture_diffuse, UV);
//    outUV = vec3(UV, 0.0);
}

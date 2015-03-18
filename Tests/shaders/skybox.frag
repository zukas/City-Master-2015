#version 330

uniform samplerCube tsampler[1];

in vec3 UV;

out vec4 outColour;

void main()
{
    outColour = vec4(0.5, 0.2, 0.6, 1.0);// texture(tsampler[0], UV);
}

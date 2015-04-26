#version 330

uniform samplerCube tsampler[1];

smooth in vec3 UV;

out vec4 outColour;

void main()
{
    outColour = texture(tsampler[0], UV);
}

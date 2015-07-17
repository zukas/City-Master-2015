#version 330

in vec2 texCoord;
out vec4 outColour;

uniform sampler2D tsampler[1];
uniform vec4 colour;

void main()
{
        vec4 textColour = texture2D(tsampler[0], texCoord);
        outColour = vec4(textColour.r, textColour.r, textColour.r, textColour.r) * colour;
}

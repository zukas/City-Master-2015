#version 330

in vec2 texCoord;
out vec4 outColour;

uniform sampler2D tsampler[1];

void main() {
    outColour = texture2D(tsampler[0], texCoord);
}

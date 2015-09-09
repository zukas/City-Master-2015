#version 330

layout (location = 0) in vec3 inPosition;
//layout (location = 1) in vec2 inUV;

uniform mat4 modelMatrix;
//uniform vec2 screen_size;

//out vec4 FXAA_pos;

//vec4 calcFXAAPos(vec2 screenSize, vec2 UV);

void main()
{
    gl_Position = modelMatrix * vec4(inPosition, 1.0);
//    FXAA_pos = calcFXAAPos(screen_size, inUV);
}

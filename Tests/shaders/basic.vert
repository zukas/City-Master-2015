#version 330

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColour;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

out vec3 colour;

void main()
{
    colour = inColour;
    mat4 MVP = (projectionMatrix * viewMatrix * modelMatrix);
    gl_Position =  MVP * vec4(inPosition, 1.0);
}


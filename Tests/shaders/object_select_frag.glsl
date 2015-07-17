#version 330

uniform vec4 objectID;

out vec4 colour;

void main()
{
    colour = objectID;
}

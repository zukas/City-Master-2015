#version 330

uniform vec3 objectID;

out vec4 colour;

void main()
{
    colour = vec4(objectID, 1.0);
}

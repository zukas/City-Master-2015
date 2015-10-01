#version 330 core

out vec4 colour;

in vec2 UV;

struct SunLight
{
    vec3 position;
    float radius;
};

uniform sampler2D position_texture;
uniform sampler2D normal_texture;
uniform sampler2D colour_texture;
uniform vec3 camera_position;

uniform SunLight lights;


void trace_ray() {

}


void main()
{

    colour = vec4(0,0,0,0);
}

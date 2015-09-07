#version 330

uniform sampler2D position_texture;
uniform sampler2D colour_texture;
uniform vec2 screen_size;

out vec4 colour;

void main()
{
    vec2 _coord = gl_FragCoord.xy / screen_size;
    vec3 _pos = texture2D(position_texture, _coord).xyz;
    vec3 _colour = texture2D(colour_texture, _coord).xyz;
    _coord = normalize(_coord);

    if(_colour.r == 0) _colour.r = _coord.x + 0.2;
    if(_colour.g == 0) _colour.g = 0.4;
    if(_colour.b == 0) _colour.b = _coord.y + 0.6;

    colour = vec4(_colour, 1.0);
}

#version 330
out vec4 colour;

uniform sampler2D position_texture;
uniform sampler2D normal_texture;
uniform sampler2D colour_texture;
uniform vec2 screen_size;
uniform int render_type;

void main()
{
    vec2 _coord = gl_FragCoord.xy / screen_size;
    vec3 _pos = texture2D(position_texture, _coord).rgb;
    vec3 _normal = texture2D(normal_texture, _coord).rgb;
    vec3 _colour = texture2D(colour_texture, _coord).rgb;
    _coord = normalize(_coord);

    if(render_type == 0) {
        colour = vec4(_pos, 1.0);
    } else if(render_type == 1) {
        colour = vec4(_normal, 1.0);
    } else if(render_type == 2) {
        colour = vec4(_colour, 1.0);
    } else {
        colour = vec4(_coord.xy, 0, 1.0);
    }
}

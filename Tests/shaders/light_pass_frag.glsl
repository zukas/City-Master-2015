#version 330
out vec4 colour;

uniform sampler2D position_texture;
uniform sampler2D normal_texture;
uniform sampler2D colour_texture;
uniform vec2 screen_size;
uniform vec3 camera_position;
uniform int render_type;



void main()
{
    vec2 _coord = gl_FragCoord.xy / screen_size;
    vec3 _pos = texture2D(position_texture, _coord).xyz;
    vec3 _normal = texture2D(normal_texture, _coord).xyz;
    vec3 _colour = texture2D(colour_texture, _coord).rgb;
//    vec3 _lights = calc_light(_pos, _normal, _colour);

    vec3 _view_direction = normalize(camera_position - _pos);
    vec3 _light_direction = normalize(-_pos);
    float _distance = length(-_pos);

    vec3 ambient = 0.15 * _colour;
    vec3 normal = normalize(_normal);

    float _diff = max(dot(_light_direction, _normal), 0.5);
    vec3 diffuse = _diff * _colour;

    vec3 _reflect_direction = reflect(-_light_direction, normal);

    vec3 specular = vec3(0.8) * pow(max(dot(-_light_direction, _reflect_direction), 0.2), 2.0);


    if(render_type == 0) {
        colour = vec4(diffuse + specular, 1.0);
    } else if(render_type == 1) {
        colour = vec4(_colour, 1.0);
    } else if(render_type == 2) {
        colour = vec4(_normal, 1.0);
    } else {
        colour = vec4(_pos, 1.0);
    }
}

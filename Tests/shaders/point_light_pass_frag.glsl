#version 330

struct PointLight
{
    vec3 position;
    vec3 colour;
    float attenuation_constant;
    float attenuation_linear;
    float attenuation_exp;
    float ambient_intensity;
    float diffuse_intensity;
};

uniform sampler2D position_texture;
uniform sampler2D colour_texture;
uniform sampler2D normal_texture;
uniform PointLight point_light;
uniform vec3 camera_position;
uniform float specular_power;
uniform float mat_specular_intensity;
uniform vec2 screen_size;

out vec4 colour;

vec2 texture_coord()
{
    return gl_FragCoord.xy / screen_size;
}

vec4 point_light_internal(vec3 light_direction, vec3 position, vec3 normal)
{
    vec4 _ambient_colour = vec4(point_light.colour, 1.0) * point_light.ambient_intensity;
    float _diffuse_factor = dot(normal, -light_direction);

    vec4 _diffuse_colour  = vec4(0, 0, 0, 0);
    vec4 _specular_color = vec4(0, 0, 0, 0);

    if (_diffuse_factor > 0.0) {

        _diffuse_colour = vec4(point_light.colour, 1.0) * point_light.diffuse_intensity * _diffuse_factor;
        vec3 _vertex_to_eye = normalize(camera_position - position);
        vec3 _light_reflection = normalize(reflect(light_direction, normal));
        float _specular_factor = dot(_vertex_to_eye, _light_reflection);
        _specular_factor = pow(_specular_factor, specular_power);
        if (_specular_factor > 0.0) {
            _specular_color = vec4(point_light.colour, 1.0) * mat_specular_intensity * _specular_factor;
        }
    }

    return (_ambient_colour + _diffuse_colour + _specular_color);
}

vec4 calc_point_light(vec3 position, vec3 normal)
{
    vec3 _light_direction = position - point_light.position;
    float _distance = length(_light_direction);
    _light_direction = normalize(_light_direction);

    vec4 _colour = point_light_internal(_light_direction, position, normal);

    float _attenuation =  point_light.attenuation_constant +
            point_light.attenuation_linear* _distance +
            point_light.attenuation_exp * _distance * _distance;

    _attenuation = max(1.0, _attenuation);

    return _colour / _attenuation;
}

void main()
{
    vec2 coord = texture_coord();
    vec3 _pos = texture2D(position_texture, coord).xyz;
    vec3 _colour = texture2D(colour_texture, coord).xyz;
    vec3 _normal = texture2D(normal_texture, coord).xyz;
    _normal = normalize(_normal);

    colour = vec4(_colour, 1.0) * calc_point_light(_pos, _normal);
}

#version 420 core

out vec4 colour;

in vec2 UV;

uniform sampler2D position_texture;
uniform sampler2D normal_texture;
uniform sampler2D colour_texture;
uniform vec3 camera_position;
uniform vec2 pix_size;
uniform int render_type;


vec4 calc_light(vec2 pix_pos) {

    float AmbientIntensity = 0.1;
    float DiffuseIntensity = 0.7;
    float SpecularIntensity = 0.05;
    float SpecularPower = 8.0;

    vec4 out_colour = vec4(0,0,0,0);

    vec3 pos = texture2D(position_texture, pix_pos).xyz;
    vec3 normal = normalize(texture2D(normal_texture, pix_pos).xyz);
    vec4 colour = vec4(texture2D(colour_texture, pix_pos).rgb, 1.0);
    if(length(normal) > 0.5) {
        vec3 light_dir = pos - normalize(pos) * 100;
        float distance_from_source = length(light_dir);
        light_dir = normalize(light_dir);
        float diffuse_factor = dot(normal, -light_dir);

        vec4 ambient = colour * AmbientIntensity;
        vec4 diffuse = vec4(0, 0, 0, 0);
        vec4 specular = vec4(0, 0, 0, 0);

        if(diffuse_factor > 0.0) {
            diffuse = colour * DiffuseIntensity * diffuse_factor;

            vec3 vert_to_eye = normalize(camera_position - pos);
            vec3 light_reflect = normalize(reflect(light_dir, normal));
            float specular_factor = dot(vert_to_eye, light_reflect);
            specular_factor = pow(specular_factor, SpecularPower);

            if (specular_factor > 0.0) {
                specular = colour * SpecularIntensity * specular_factor;
            }
        }

        out_colour = colour * (ambient + diffuse + specular) * (8.1 - exp(pow(distance_from_source, 0.083)));
    }

    if(render_type == 1) {
        out_colour = vec4(pos,1.0);
    } else if(render_type == 2) {
        out_colour = vec4(normal,1.0);
    } else if(render_type == 3) {
        out_colour = vec4(colour.rgb,1.0);
    }


    return out_colour;
}

void main()
{
    vec2 tmp_pix_size = pix_size;

    vec4 tmp_colour = calc_light(UV);

    colour = tmp_colour;
}

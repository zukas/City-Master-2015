#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform vec3 lightPosition;

out vec2 UV;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

void main()
{
    UV = inUV;

    vec4 tmpPos = modelMatrix * viewMatrix * vec4(inPosition,1);
    Position_worldspace = tmpPos.xyz;

    vec4 other_pos = vec4(0.1, 0.1, 0.2, 1.0);

    vec4 tmpPosCam = modelMatrix * viewMatrix * vec4(inPosition,1);
    vec3 PosCam = tmpPosCam.xyz;

    EyeDirection_cameraspace = vec3(tmpPosCam - tmpPos).xyz;

    vec4 tmpLightPos = modelMatrix * viewMatrix * vec4(lightPosition ,1);
    LightDirection_cameraspace = (tmpLightPos - tmpPosCam).xyz;

    vec4 tmpNormal = modelMatrix * vec4(inNormal,0);
    Normal_cameraspace = normalize(tmpNormal).xyz;

    mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;
    gl_Position = MVP * vec4(inPosition, 1.0);
}


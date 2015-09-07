#version 330

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 position;
out vec3 normal;
out vec2 UV;

void main()
{
    vec4 tmp_normal = modelMatrix * vec4(inNormal, 0.0);
    vec4 tmp_world = modelMatrix * vec4(inPosition, 1.0);
    position = tmp_world.xyz;
    normal = tmp_normal.xyz;
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(inPosition, 1.0);
    UV = inUV;
}
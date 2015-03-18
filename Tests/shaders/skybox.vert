#version 330

layout(location = 0) in vec3 inPosition;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 cameraPosition;

out vec3 UV;

void main()
{

    UV = vec3(inPosition.x, -inPosition.yz);
    gl_Position = mat4(1.0) * viewMatrix * projectionMatrix * vec4(inPosition - cameraPosition, 1.0);
}

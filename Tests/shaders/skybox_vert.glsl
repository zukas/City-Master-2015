#version 330

layout(location = 0) in vec3 inPosition;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

smooth out vec4 UV;

void main()
{
    UV = inPosition;
    vec4 VPp =  projectionMatrix * viewMatrix * vec4(inPosition, 1);
    vec4 nVPp = normalize(VPp);
    VPp.xyw
    gl_Position = vec4(VPp.xy, nVPp.z, VPp.w);
}

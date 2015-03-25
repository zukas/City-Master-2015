#version 330

layout(location = 0) smooth in vec3 inPosition;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 cameraPosition;

smooth out vec3 UV;

void main()
{

//    vec4 tmpPosCam = viewMatrix * vec4(inPosition,1);
//    vec3 PosCam = tmpPosCam.xyz;

//    vec3 EyeDirection_cameraspace = vec3(0,0,0) - PosCam;

//    mat4 inverseProjection = inverse(projectionMatrix);
//    mat3 inverseModelview = transpose(mat3(viewMatrix));
//    vec3 unprojected = (inverseProjection * inPosition).xyz;


//    UV = normalize(vec3(inPosition.x, -inPosition.yz));

    UV = inPosition;

    vec4 VPp =  projectionMatrix * viewMatrix * vec4(inPosition, 1);
    gl_Position =  vec4(VPp.xy, normalize(VPp.z), VPp.w);

//    UV = inPosition;// vec3(inPosition.x, -inPosition.yz);
//    gl_Position = mat4(1.0) * viewMatrix * projectionMatrix * vec4(inPosition + cameraPosition, 1.0);
}

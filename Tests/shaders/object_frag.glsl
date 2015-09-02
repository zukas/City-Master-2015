#version 330

uniform sampler2D texture;
uniform int objectId;
uniform vec3 lightPosition;
uniform float lightStrength;
uniform int objectSelected;

in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

out vec4 colour;

void main()
{
    if(objectId == 0) {
        colour = texture2D(texture, UV);
    } else {

    }

//    colour = vec4(UV.x, UV.y, UV.y, 1.0);

//    for(int i = 0; i < tcount; i++)
//    {
//        if(i == 0)
//        {
//            tmpc+=texture2D(tsampler[0], UV);
//        }
//        else if(i == 1)
//        {
//            tmpc+=texture2D(tsampler[1], UV);
//        }
//        else if(i == 2)
//        {
//            tmpc+=texture2D(tsampler[2], UV);
//        }
//        else if(i == 3)
//        {
//            tmpc+=texture2D(tsampler[3], UV);
//        }
//        else if(i == 4)
//        {
//            tmpc+=texture2D(tsampler[4], UV);
//        }
//    }
//    if(tcount == 0)
//    {
//        tmpc = vec4(UV.x, UV.y, 0.35, 1.0);
//    }

//    vec3 LightColor = vec3(1,0.92,0.90);
//    if(objectSelected == 1)
//    {
//        LightColor = vec3(0.89,0.42,0.48);
//    }
//    float LightPower = length( lightPosition ) * lightStrength;

//    vec3 MaterialDiffuseColor = tmpc.rgb;
//    vec3 MaterialAmbientColor = vec3(0.82,0.82,0.82) * MaterialDiffuseColor;
//    vec3 MaterialSpecularColor = vec3(0.25,0.25,0.25);

//    float distance = length( lightPosition - Position_worldspace );

//    vec3 n = normalize( Normal_cameraspace );
//    vec3 l = normalize( LightDirection_cameraspace );
//    float cosTheta = clamp( dot( n,l ), 0.1, 1 );

//    vec3 E = normalize(EyeDirection_cameraspace);
//    vec3 R = reflect(-l,n);
//    float cosAlpha = clamp(dot( E,R ), 0, 1);

//    colour = MaterialAmbientColor * LightColor +
//            MaterialDiffuseColor * LightColor  * cosTheta / (distance*distance) +
//            MaterialSpecularColor * LightColor  * pow(cosAlpha,5) / (distance*distance);


}

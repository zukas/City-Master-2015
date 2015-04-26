#version 330

layout(points) in;
layout(points) out;
layout(max_vertices = 40) out;

// All that we get from vertex shader

in vec3 vPositionPass[1];
in vec3 vVelocityPass[1];
in vec3 vColorPass[1];
in float fLifeTimePass[1];
in float fSizePass[1];
in int iTypePass[1];
in int reusePass[1];

// All that we send further

out vec3 vPositionOut;
out vec3 vVelocityOut;
out vec3 vColorOut;
out float fLifeTimeOut;
out float fSizeOut;
out int iTypeOut;
out int reuseOut;

uniform vec3 vGenPosition; // Position where new particles are spawned
uniform vec3 vGenGravityVector; // Gravity vector for particles - updates velocity of particles 
uniform vec3 vGenVelocityMin; // Velocity of new particle - from min to (min+range)
uniform vec3 vGenVelocityRange;

uniform vec3 vGenColor;
uniform float fGenSize; 
uniform float fDim;

uniform float fGenLifeMin, fGenLifeRange; // Life of new particle - from min to (min+range)
uniform float fTimePassed; // Time passed since last frame

uniform vec3 vRandomSeed; // Seed number for our random number function
vec3 vLocalSeed;

uniform int iNumToGenerate; // How many particles will be generated next time, if greater than zero, particles are generated

//varying vec3 v_texCoord3D;

highp float rand(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt= dot(co.xy ,vec2(a,b));
    highp float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}

highp float rand_pos(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt= dot(co.xy ,vec2(a,b));
    highp float sn= mod(dt,3.14);
    return (fract(sin(sn) * c) + 1.0) / 2.f;
}

vec3 rand_v3(vec3 co, float axel)
{
    return vec3(rand(co.xy), rand(co.xz), rand(co.yz)) * axel;
}

vec3 rand_v3_pos(vec3 co, float axel)
{
    return vec3(rand_pos(co.xz), rand_pos(co.yz), rand_pos(co.xy)) * axel;
}

#define PI 3.14159
#define PI2 PI * 2.0
#define INC PI / 8.0
#define INC2 PI2 / 8.0

void main()
{
    float radius = 2;

    float update = fTimePassed / 1000.0;
    vPositionOut = vPositionPass[0];
    vVelocityOut = vVelocityPass[0];

    vPositionOut = vGenPosition + vec3(vVelocityOut.z * cos(vVelocityOut.x) * sin(vVelocityOut.y),
                        vVelocityOut.z * sin(vVelocityOut.x) * sin(vVelocityOut.y),
                        vVelocityOut.z * cos(vVelocityOut.y));
    vVelocityOut.x += rand_pos(vRandomSeed.xy) * 0.5 * update;
    vVelocityOut.y += rand_pos(vRandomSeed.xy) * 0.5 * update;

    vColorOut = vColorPass[0];


    fLifeTimeOut = fLifeTimePass[0] - fTimePassed;
    fSizeOut = fSizePass[0];
    iTypeOut = iTypePass[0];
    reuseOut = reusePass[0];

    vColorOut.x = (fGenLifeRange - fLifeTimeOut) / fGenLifeRange;
    vColorOut.y = (fGenLifeRange - fLifeTimeOut) / fGenLifeRange;
    vColorOut.z =  fLifeTimeOut / fGenLifeRange;

    if(iTypeOut == 0)
    {
        EmitVertex();
        EndPrimitive();


        for(int i = 0; i < iNumToGenerate; ++i)
        {

            float a = (PI2 / iNumToGenerate ) * (iNumToGenerate + 1 - i) * PI2;
            float b = (PI / iNumToGenerate) * (iNumToGenerate + 1 - i) * PI;

            float rand_xy = rand_pos(vRandomSeed.xy) * a;
            float rand_z = rand_pos(vRandomSeed.xy) * b;

                vPositionOut = vGenPosition + vec3(radius * cos(rand_xy) * sin(rand_z), radius * sin(rand_xy) * sin(rand_z), radius * cos(rand_z));
                vVelocityOut = vec3(rand_xy ,rand_z, radius);
                vColorOut = vGenColor;
                fLifeTimeOut = fGenLifeRange;// + fGenLifeMin*randZeroOne();
                fSizeOut = fGenSize;
                iTypeOut = 1;
                reuseOut = 0;
                EmitVertex();
                EndPrimitive();
        }
    }
    else if(fLifeTimeOut > 0.0)
    {
        EmitVertex();
        EndPrimitive();
    }
    else if(reuseOut < 10)
    {


            float a = PI2 * rand_pos(vRandomSeed.xy);
            float b = PI * rand_pos(vRandomSeed.xy);

            float rand_xy = rand_pos(vRandomSeed.xy) * a;
            float rand_z = rand_pos(vRandomSeed.xy) * b;

                vPositionOut = vGenPosition + vec3(radius * cos(rand_xy) * sin(rand_z), radius * sin(rand_xy) * sin(rand_z), radius * cos(rand_z));
                vVelocityOut = vec3(rand_xy ,rand_z, radius);
                vColorOut = vGenColor;
                fLifeTimeOut = fGenLifeRange;// + fGenLifeMin*randZeroOne();
                fSizeOut = fGenSize;
                iTypeOut = 1;
                reuseOut += 1;
                EmitVertex();
                EndPrimitive();

    }
}

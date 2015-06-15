#version 330

layout(points) in;
layout(points) out;
layout(max_vertices = 30) out;

// All that we get from vertex shader

in vec3 vPositionPass[];
in vec3 vVelocityPass[];
in vec3 vColorPass[];
in float fLifeTimePass[];
in float fSizePass[];
in float iTypePass[];
in float reusePass[];

// All that we send further

out vec3 vPositionOut;
out vec3 vVelocityOut;
out vec3 vColorOut;
out float fLifeTimeOut;
out float fSizeOut;
out float iTypeOut;
out float reuseOut;

uniform vec3 vGenPosition; // Position where new particles are spawned
uniform vec3 vGenGravityVector; // Gravity vector for particles - updates velocity of particles 
uniform vec3 vGenVelocityMin; // Velocity of new particle - from min to (min+range)
uniform vec3 vGenVelocityRange;

uniform vec3 vGenColor;
uniform float fGenSize; 
uniform float fDim;
uniform float ttime;

uniform float fGenLifeMin, fGenLifeRange; // Life of new particle - from min to (min+range)
uniform float fTimePassed; // Time passed since last frame

uniform vec3 vRandomSeed; // Seed number for our random number function
vec3 vLocalSeed;

uniform int iNumToGenerate; // How many particles will be generated next time, if greater than zero, particles are generated
uniform sampler1D tsampler[1];
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

vec3 rand_v3_2(float TexCoord)
{
    vec3 r = texture(tsampler[0], TexCoord).xyz;
    r = rand_v3(r, 1);
    r -= vec3(0.5, 0.5, 0.5);
    return r;
}

vec3 some_rand()
{
    vec3 val = vRandomSeed / ttime;
    val.x = rand(val.xy);
    val.y = rand(val.yz);
    val.z = rand(val.xy);
    val = rand_v3(val, 1.0);
    return val;
}

#define PI 3.14159
#define PI2 PI * 2.0
#define INC PI / 8.0
#define INC2 PI2 / 8.0
#define P_LAUNCH 0.0f
#define P_SHELL 1.0f
#define P_SHELL2 2.0f

void main()
{
     float Age = fLifeTimePass[0] + fTimePassed;

     if(iTypePass[0] == P_LAUNCH)
     {
        if(Age > fGenLifeRange)
        {
            for (int i = 0 ; i < iNumToGenerate ; i++)
            {
                vPositionOut = vGenPosition + some_rand() * 20;
                vec3 r = some_rand();
//                r.y = max(r.y, 0.3);
                vVelocityOut = normalize(r) + some_rand() * 3;
                vColorOut = vGenColor;
                fLifeTimeOut = 0.0;
                fSizeOut = fGenSize;
                iTypeOut = P_SHELL;
                reuseOut = P_LAUNCH;
                EmitVertex();
                EndPrimitive();
            }
            Age = 0.0;
        }
        vPositionOut = vGenPosition;
        vVelocityOut = vGenVelocityRange;
        vColorOut = vGenColor;
        fLifeTimeOut = Age;
        fSizeOut = fGenSize;
        iTypeOut = P_LAUNCH;
        reuseOut = P_LAUNCH;
        EmitVertex();
        EndPrimitive();
     }
     else
     {
        float DeltaTimeSecs = fTimePassed / 1000.0f;
        float t1 = fLifeTimePass[0] / 1000.0;
        float t2 = Age / 1000.0;
        vec3 DeltaP = DeltaTimeSecs * vVelocityPass[0];
        vec3 DeltaV = some_rand() * DeltaTimeSecs;
        if (iTypePass[0] == P_SHELL)
        {
            if (Age < fGenLifeMin)
            {
                iTypeOut = P_SHELL;
                vPositionOut = vPositionPass[0] + DeltaP;
                vVelocityOut = vVelocityPass[0] + DeltaV;
                vColorOut = vColorPass[0];
                fLifeTimeOut = Age;
                fSizeOut = fSizePass[0];
                reuseOut = reusePass[0];
                EmitVertex();
                EndPrimitive();
            }
            else
            {
                for (int i = 0 ; i < iNumToGenerate ; i++)
                {
                    iTypeOut = P_SHELL2;
                    vPositionOut = vPositionPass[0] + some_rand();
                    vec3 r = some_rand();
                    vVelocityOut = normalize(r) * 2 + some_rand() * 5;
                    fLifeTimeOut = 0.0f;
                    fSizeOut = fSizePass[0];
                    reuseOut = reusePass[0];
                    vColorOut = vColorPass[0];
                    EmitVertex();
                    EndPrimitive();
                }
            }
        }
        else
        {
            if (Age < fGenLifeRange)
            {
                iTypeOut = P_SHELL2;
                vPositionOut = vPositionPass[0] + DeltaP;
                vVelocityOut = vVelocityPass[0] + DeltaV;
                vColorOut = vColorPass[0];
                fLifeTimeOut = Age;
                fSizeOut = fSizePass[0];
                reuseOut = reusePass[0];
                EmitVertex();
                EndPrimitive();
            }
        }
     }
}

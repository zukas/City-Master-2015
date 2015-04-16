#version 330

uniform sampler2D tsampler[1];
uniform int tcount;

smooth in vec2 vTexCoord;
flat in vec4 vColorPart;
//in vec4 gl_FragCoord;

out vec4 FragColor;

void main()
{
    vec2 uv = vTexCoord.xy;
    uv.y *= -1.0;
    vec3 vTexColor = texture2D(tsampler[0], uv).rgb;
    float alpha = 0.0;
    for(float i = 0.01; i < 1.0; i+=i)
    {
        if(vTexColor.r > i && vTexColor.g > i && vTexColor.b > i)
        {
            alpha = i;
        }
    }
    if(alpha < 0.05)
    {
        discard;
    }
    else
    {
        alpha -= gl_FragCoord.z * 0.01;

        FragColor = vec4(vTexColor, alpha) * vColorPart;
    }

}

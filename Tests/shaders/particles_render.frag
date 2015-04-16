#version 330

uniform sampler2D tsampler[1];
uniform int tcount;

smooth in vec2 vTexCoord;
flat in vec4 vColorPart;

out vec4 FragColor;

void main()
{
    vec2 uv = vTexCoord.xy;
    uv.y *= -1.0;
    vec3 vTexColor = texture2D(tsampler[0], uv).rgb;
    float alpha = 0.0;
    for(float i = 0.02; i < 1.0; i+=i)
    {
        if(vTexColor.r > i && vTexColor.g > i && vTexColor.b > i)
        {
            alpha = i;
        }
    }
    FragColor = vec4(vTexColor, alpha) * vColorPart;
//  FragColor = vColorPart;
}

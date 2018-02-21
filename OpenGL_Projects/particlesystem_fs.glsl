#version 430
layout(location = 1) uniform float time;
layout(location = 2) uniform vec2 resolution;
layout(location = 3) uniform vec2 mouse;
layout(location = 4)uniform vec4 fcolor;

out vec4 fragcolor;            
in float age_out;

void main()
{
    float r = length(gl_PointCoord.st - vec2(0.5));
    float s = smoothstep(0.5,0.3,r);

    vec4 color0 = vec4(0.7,0.4,0.3,0.7*s);
    vec4 color1 = vec4(0.7,0.1,0.0,0.3*s);
    fragcolor = mix(color1,color0,age_out/500.0);
}
#version 430

layout(location = 1) uniform float time;
layout(location = 2) uniform vec2 resolution;
layout(location = 3) uniform vec2 mouse;
layout(location = 4)uniform vec4 fcolor;

out vec4 fragColor;

void main()
{
    fragColor = fcolor;
}
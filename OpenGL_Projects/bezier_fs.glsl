#version 460
layout(location = 1) uniform float time;
layout(location = 2) uniform vec2 resolution;
layout(location = 3) uniform vec2 mouse;

out vec4 fragColor;

void main()
{
    fragColor = vec4(1.0,0.0,0.0,1.0);
}
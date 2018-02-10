#version 430
layout(location = 1) uniform float time;
layout(location = 2) uniform vec2 resolution;
layout(location = 3) uniform vec2 mouse;
layout(location = 4)uniform vec4 fcolor;

out vec4 fragColor;

void main()
{
    vec2 uv = gl_FragCoord.xy/resolution.xy;
    vec2 mouse_uv = mouse.xy/resolution.xy;
    vec3 bg = vec3(250.0,243.0,225.0)/vec3(255.0);

    vec3 color = bg;
    fragColor = vec4(1.0);
}
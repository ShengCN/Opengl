#version 460
layout(location = 1) uniform float time;
layout(location = 2) uniform vec2 resolution;
layout(location = 3) uniform vec2 mouse;

out vec4 fragColor;

uniform int pass;

void main()
{
    vec3 color = vec3(0.0);
    if(pass == 1)
        color = vec3(1.0,0.0,0.0);
    else if(pass == 2)
        color= vec3(0.0,1.0,0.0);
    else 
        color = vec3(0.0,0.0,1.0);

    fragColor = vec4(color,1.0);
}
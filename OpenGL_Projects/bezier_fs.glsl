#version 430
layout(location = 1) uniform float time;
layout(location = 2) uniform vec2 resolution;
layout(location = 3) uniform vec2 mouse;

out vec4 fragColor;

in float pointID; // only allow 100 control points

uniform int pass;

void main()
{
    vec3 color = vec3(0.0);
    if(pass == -1)                      // Detect mouse
       // color = vec3(pointID);
       color = vec3(pointID*0.1);
    if(pass == 1)                       // Bezier
        color = vec3(1.0,0.0,0.0);
    else if(pass == 2)                  // Control points
       //color= vec3(0.0,1.0,0.0);
        color= vec3(0.0,1.0,0.0);
    else if( pass ==3)                  // Control polygon
        color = vec3(0.0,0.0,1.0);

    fragColor = vec4(color,1.0);
}
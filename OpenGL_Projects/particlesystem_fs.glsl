#version 430
layout(location = 1) uniform float time;
layout(location = 2) uniform vec2 resolution;
layout(location = 3) uniform vec2 mouse;
layout(location = 4)uniform vec4 fcolor;

out vec4 fragcolor;            
// in float age_out;
// in vec4 pos_out;
// in vec4 camera_pos;

void main()
{
    float r = length(gl_PointCoord.st - vec2(0.5));
    float s = step(0.5,r);
	
	fragcolor = vec4(s)*0.7;
    
}
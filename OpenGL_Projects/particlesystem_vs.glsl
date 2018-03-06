#version 430            

uniform mat4 PVM;
uniform float time;
uniform float slider;
uniform mat4 V;

layout(location = 0) in vec3 pos_attrib;
layout(location = 1) in vec3 vel_attrib;
layout(location = 2) in float age_attrib;

out vec3 pos_out; 
out vec3 vel_out; 
out float age_out;
out vec4 camera_pos;

vec3 Map2Coord(int id)
{
	int x = (gl_InstanceID) % 256;
	int y = (gl_InstanceID / (256)) % 256;
	int z = (gl_InstanceID / (256*256)) % 256;
	return vec3(x-256/2,y-256/2,z-256/2)/256 * 10.0 - vec3(0.0,5.0,0.0);
}

void main()
{
    gl_Position = PVM * vec4(Map2Coord(gl_InstanceID),1.0);
    gl_PointSize = 10.0;

	pos_out = pos_attrib;
	vel_out = vel_attrib;
	age_out = age_attrib;
    camera_pos = inverse(V) * vec4(0.0,0.0,0.0,1.0);
}


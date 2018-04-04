#version 430
layout(location = 0) in vec3 pos_attrib;
layout(location = 1) in vec2 tex_coord_attrib;
layout(location = 2) in vec3 normal_attrib;

uniform mat4 PVM;
uniform mat4 V;
uniform mat4 M;

out vec2 texture_coord;
out vec3 normal_coord;

void main()
{
	gl_Position = PVM * vec4(pos_attrib,1.0);
	texture_coord = tex_coord_attrib;
	normal_coord = normal_attrib;
}
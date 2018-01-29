#version 400
uniform mat4 PVM;
 
layout(location = 0) in vec3 pos_attrib;
layout(location = 1) in vec2 tex_coord_attrib;
layout(location = 2) in vec3 normal_attrib;
layout(location = 3) in mat4 displace;
layout(location = 7) in vec4 crt_fish_color;

out vec2 tex_coord;
out vec4 fish_color;


void main()
{

    gl_Position = PVM * displace *vec4(pos_attrib,1.0);

    tex_coord = tex_coord_attrib;
	fish_color = crt_fish_color;
}
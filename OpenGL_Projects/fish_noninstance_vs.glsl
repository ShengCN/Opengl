#version 430
uniform mat4 PVM;
 
layout(location = 0) in vec3 pos_attrib;
layout(location = 1) in vec2 tex_coord_attrib;
layout(location = 2) in vec3 normal_attrib;

out vec2 tex_coord;
out vec4 fish_color;

layout(location = 2) uniform mat4 dis;

void main()
{
    gl_Position = PVM * dis * vec4(pos_attrib,1.0);

    tex_coord = tex_coord_attrib;
}
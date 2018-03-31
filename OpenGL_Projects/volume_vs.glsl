#version 430            

layout(location = 0) uniform mat4 PV;
layout(location = 1) uniform int pass; //which render pass is this?
layout(location = 4) uniform mat4 M;
layout(location = 5) uniform mat4 T;

layout(location = 0) in vec3 pos_attrib;
layout(location = 1) in vec2 tex_coord_attrib;
layout(location = 2) in vec3 normal_attrib;

out vec3 vpos; 
out vec2 textureCoord;

void main(void)
{
	gl_Position = PV*M*vec4(pos_attrib, 1.0);
	vpos = vec3(T*M*vec4(pos_attrib, 1.0));
}
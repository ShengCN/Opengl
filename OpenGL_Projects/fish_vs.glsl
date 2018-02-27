#version 430
layout(location = 0) in vec3 pos_attrib;
layout(location = 1) in vec2 tex_coord_attrib;
layout(location = 2) in vec3 normal_attrib;

out vec2 tex_coord;
out vec3 fish_pos;
out vec3 fish_norm;
out vec3 camera;

uniform mat4 PVM;
uniform mat4 V;
uniform mat4 M;

const vec4 origin = vec4(0.0,0.0,0.0,1.0);

void main()
{
    gl_Position = PVM *vec4(pos_attrib,1.0);
    tex_coord = tex_coord_attrib;

    camera = vec3(inverse(V)*origin);
    fish_pos = gl_Position.xyz;
    fish_norm = vec3(transpose(inverse(M))*vec4(normal_attrib,0.0));
}
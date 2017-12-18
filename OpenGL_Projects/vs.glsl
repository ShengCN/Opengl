#version 400

<<<<<<< HEAD
in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;
in vec3 sphere_offset;

out DATA_VS
{
    float id;
    vec2 tex_coord;
    vec4 norm_coord;
    vec4 pos_coord;
    vec3 camera_position;
} vs_out;

uniform float time;
uniform mat4 PVM;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

const vec4 origin = vec4(0.0,0.0,0.0,1.0);

void main()
{
    vec3 dis = vec3(sin(time),0.0,0.0);
    gl_Position = PVM* vec4(pos_attrib+dis,1.0);
=======
int main()
{
    gl_Position = vec4(1.0);
>>>>>>> 6d2ea0ea3de1622d3487dfc54902fff3a9e5d880
}
#version 400

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
uniform vec3 light_position;
uniform mat4 PVM;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform float slider;

const vec4 origin = vec4(0.0,0.0,0.0,1.0);

void main()
{
    vec3 dis = vec3(sin(time)  * slider,0.0,0.0);
    gl_Position = PVM* vec4(pos_attrib+vec3(light_position),1.0);

    // vs out
    vs_out.camera_position = vec3(inverse(V) * origin);
}
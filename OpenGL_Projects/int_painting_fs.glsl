#version 430

uniform sampler2D diffuse_color;

in vec2 texture_coord;
in vec3 normal_coord;

out vec4 fragcolor;


void main()
{
    fragcolor = vec4(vec3(0.5),0.8);
}
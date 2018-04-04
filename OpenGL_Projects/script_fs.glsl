#version 430

uniform sampler2D diffuse_color;
uniform float slider;
in vec2 texture_coord;
in vec3 normal_coord;

out vec4 fragcolor;


void main()
{
    fragcolor = vec4(normal_coord.xyz + 0.2,1.0);
}
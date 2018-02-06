#version 430

uniform sampler2D diffuse_color;

in vec2 texture_coord;
in vec4 normal_coord;

out vec4 fragcolor;


void main()
{
    fragcolor = vec4(0.0,1.0,0.0,1.0);
}
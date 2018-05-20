#version 430

uniform sampler2D diffuse_color;

in vec2 texture_coord;
in vec3 normal_coord;
in vec3 vs_color;

out vec4 fragcolor;

uniform float slider;
void main()
{
    // fragcolor = vec4(vec3(0.5),1.0f);
    fragcolor = vec4(vs_color,1.0);
}
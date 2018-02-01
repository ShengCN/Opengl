#version 430

uniform sampler2D diffuse_color;

out vec4 fragcolor;
in vec2 tex_coord;
in vec4 fish_color;

layout(location = 3) uniform bool isInstanced;
layout(location = 4) uniform vec4 color;

void main()
{
        fragcolor = mix(texture(diffuse_color,tex_coord),color,0.3);
}
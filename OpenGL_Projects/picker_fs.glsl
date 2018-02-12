#version 460
layout(location = 1) uniform float time;
layout(location = 2) uniform vec2 resolution;
layout(location = 3) uniform vec2 mouse;
layout(location = 4)uniform vec4 fcolor;
uniform int pass;
uniform sampler2D diffuse_color;

in vec2 tex_coord;
in vec3 normal; 
out vec4 fragColor;

void main()
{
    fragColor = texture(diffuse_color,tex_coord);
}
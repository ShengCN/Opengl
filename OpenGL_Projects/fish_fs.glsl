#version 400

uniform sampler2D diffuse_color;

out vec4 fragcolor;
in vec2 tex_coord;

void main()
{
    fragcolor = texture(diffuse_color,tex_coord);
    //fragcolor = vec4(0.0,1.0,1.0,1.0);
}
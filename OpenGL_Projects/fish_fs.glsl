#version 400

uniform sampler2D diffuse_color;

out vec4 fragcolor;
in vec2 tex_coord;
in vec4 fish_color;

void main()
{
    fragcolor = mix(texture(diffuse_color,tex_coord),fish_color,0.5);
    //fragcolor = vec4(0.0,1.0,1.0,1.0);

    //fragcolor = texture(diffuse_color,tex_coord);
}
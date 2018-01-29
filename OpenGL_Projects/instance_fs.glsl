#version 430

in vec3 colors;
out vec4 fragColor;

void main()
{
    fragColor = vec4(colors,1.0);
}
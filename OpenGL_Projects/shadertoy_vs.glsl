#version 430

layout(location = 1) in vec3 pos_attrib;

void main()
{
    gl_Position = vec4(pos_attrib,1.0);
}
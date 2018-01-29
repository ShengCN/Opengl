#version 430

layout(location = 0) in vec3 pos_attrib;
layout(location = 1) in vec3 displace;

uniform mat4 PVM;

out vec3 colors;

void main()
{
    gl_Position = PVM * vec4(pos_attrib + displace,1.0);
    // colors = displace;
}
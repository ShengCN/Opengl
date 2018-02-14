#version 430
layout(location = 0) in vec3 pos_attrib;
in float id;

layout(location = 0) uniform mat4 PVM;

out float pointID;

void main()
{
    gl_Position = PVM * vec4(pos_attrib,1.0);
    pointID = id;
}
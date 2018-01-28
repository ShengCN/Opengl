#version 430

layout(location = 0) uniform mat4 PVM;
layout(location = 1) uniform float time;
layout(location = 2) uniform vec2 resolution;
layout(location = 3) uniform vec2 mouse;

layout(location = 1) in vec3 pos_attrib;
layout(location = 2) in vec3 offset;

void main()
{
    gl_Position = PVM* vec4(pos_attrib+offset,1.0);
}
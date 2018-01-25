#version 430            
layout(location = 0) uniform mat4 PVM;
layout(location = 1) uniform float time;
layout(location = 2) uniform vec2 resolution;
layout(location = 3) uniform vec2 mouse;

layout(location = 1) in vec3 pos_attrib;
layout(location = 2) in vec2 tex_coord_attrib;
layout(location = 3) in vec3 normal_attrib;

out vec2 tex_coord;  

void main(void)
{
    float id = 0.1*float(gl_VertexID);
    float new_time = time * 0.1;


    vec3 dis = vec3(0.0);
    dis.z = 0.2*sin(pos_attrib.x*10+time);

    gl_Position = PVM*vec4(pos_attrib+dis, 1.0);
    //float r = 5.0*sin(10.0*id);
    //gl_Position = PVM*vec4(r*cos(id+new_time), 4.0*fract(0.1*id+new_time)-2.0, r*sin(id+new_time), 1.0);  
    
    tex_coord = tex_coord_attrib;
    
    //gl_PointSize = 10+40*pow(abs(sin(gl_Position.x*10 + time)),2);

}
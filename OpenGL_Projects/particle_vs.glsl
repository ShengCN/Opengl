#version 400            
uniform mat4 PVM;
uniform float time;
uniform vec2 resolution;
uniform vec2 mouse;

in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;

out vec2 tex_coord;  

void main(void)
{
    float id = 0.1*float(gl_VertexID);
    float new_time = time * 0.1;


    vec3 dis = vec3(0.0);
    dis.z = 0.2*sin(pos_attrib.x*10+time);

    gl_Position = PVM*vec4(pos_attrib+dis, 1.0);

    float r = 5.0*sin(10.0*id);
    gl_Position = PVM*vec4(r*cos(id+new_time), 4.0*fract(0.1*id+new_time)-2.0, r*sin(id+new_time), 1.0);     //w = 1 becase this is a point
   
    
    tex_coord = tex_coord_attrib;
    
    gl_PointSize = 10+40*pow(abs(sin(gl_Position.x*10 + time)),2);

    // // Mouse
    // vec2 mouseUV = mouse.xy / resolution.xy;
    // mouseUV.y = 1.0 - mouseUV.y;
    // float r = length(gl_Position - vec4(mouseUV.x,mouseUV.y,0.0,1.0));
    
    // if(r <0.9)
    // {
    //     gl_PointSize = 80;  
    // }

}
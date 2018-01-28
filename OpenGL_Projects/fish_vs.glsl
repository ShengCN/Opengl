#version 400
uniform mat4 PVM;
 
layout(location = 0) in vec3 pos_attrib;
layout(location = 1) in vec2 tex_coord_attrib;
layout(location = 2) in vec3 normal_attrib;
<<<<<<< HEAD
layout(location = 3) in vec4 crt_fish_color;
layout(location = 4) in vec3 displace;
=======
layout(location = 3) in vec3 pos_vec ;
layout(location = 4) in vec4 crt_fish_color;


uniform vec3 offset;
>>>>>>> Instance

out vec2 tex_coord;
out vec4 fish_color;

uniform vec3 dis;

void UseUniform()
{
    if(gl_InstanceID == 0)
        gl_Position = PVM * vec4(pos_attrib+dis,1.0);
    else
        gl_Position = PVM * vec4(pos_attrib+displace,1.0);
}

void main()
<<<<<<< HEAD
{

    //gl_Position = PVM * vec4(pos_attrib+displace,1.0);

    UseUniform();
=======
{ 
    if(gl_InstanceID == 0)
        gl_Position = PVM * vec4(pos_attrib+pos_vec+offset,1.0);
    else
        gl_Position = PVM * vec4(pos_attrib+pos_vec,1.0);
>>>>>>> Instance
    tex_coord = tex_coord_attrib;
	fish_color = crt_fish_color;
}
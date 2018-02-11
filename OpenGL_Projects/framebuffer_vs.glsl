#version 430
layout(location = 0) uniform mat4 PVM;
uniform int pass; 

layout(location = 0) in vec3 pos_attrib;
layout(location = 1) in vec2 tex_coord_attrib;
layout(location = 2) in vec3 normal_attrib;

out vec2 tex_coord; 
out vec3 normal; 

void main()
{
   	if(pass == 1) //render mesh to texture
	{
	   gl_Position = PVM*vec4(pos_attrib, 1.0);
	   tex_coord = tex_coord_attrib;
	   normal = normal_attrib;
	}
	else if(pass == 2) // render textured quad to back buffer
	{
		//full screen quad is in clip coords, so no matrix mult needed when assigning to gl_Position
		gl_Position = vec4(pos_attrib, 1.0);

		//quad doesn't have tex coords, but we can generate them since quad goes from -1.0 to +1.0 in each direction
		tex_coord = 0.5*pos_attrib.xy + vec2(0.5);
	}
}
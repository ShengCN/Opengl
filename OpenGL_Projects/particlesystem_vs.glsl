#version 430            

uniform mat4 PVM;
uniform float time;
uniform float slider;
uniform mat4 V;
uniform vec3 sourcePos;
uniform vec3 sourcePos2;

layout(location = 0) in vec3 pos_attrib;
layout(location = 1) in vec3 vel_attrib;
layout(location = 2) in float age_attrib;

// out vec3 pos_out; 
// out vec3 vel_out; 
// out float age_out;
out vec4 camera_pos;

const int MAX = 256;
const float R = 0.3;

struct PotentialSource
{
	float val;
	vec3 pos;
};

PotentialSource source1 = PotentialSource(0.5f,sourcePos*0.05);
PotentialSource source2 = PotentialSource(0.5f,sourcePos2*0.05);

vec3 Map2Coord(int id)
{
	int x = (gl_InstanceID) % MAX;
	int y = (gl_InstanceID / (MAX)) % MAX;
	int z = (gl_InstanceID / (MAX*MAX)) % MAX;
	return vec3(x-MAX/2,y-MAX/2,z-MAX/2)/MAX;
}

float Blend_Func(float r)
{
	return 1.0 - 4.0  / 9.0 * pow(r, 6) / pow(R, 6) 
			   + 17.0 / 9.0 * pow(r, 4) / pow(R, 4) 
		       - 22.0 / 9.0 * pow(r, 2) / pow(R, 2);
}

void main()
{
	vec3 pos = Map2Coord(gl_InstanceID);
	float potential = Blend_Func(length((pos-source1.pos))) * source1.val;
	potential += Blend_Func(length((pos-source2.pos))) * source2.val;

    gl_Position = PVM * vec4(pos,1.0);

	if(potential > 0.4 )
    	gl_PointSize = 10.0;
	else
		gl_PointSize =  0.0;

	// pos_out = pos_attrib;
	// vel_out = vel_attrib;
	// age_out = age_attrib;
    camera_pos = inverse(V) * vec4(0.0,0.0,0.0,1.0);
}


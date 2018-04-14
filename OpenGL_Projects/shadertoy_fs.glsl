#version 430
layout(location = 1) uniform float iTime;
layout(location = 2) uniform vec2 iResolution;
layout(location = 3) uniform vec2 iMouse;
layout(location = 4)uniform vec4 fcolor;

uniform sampler2D volume_tex;
uniform int slice;
uniform float slider;

out vec4 fragColor;

float sdSphere(vec3 p)
{
	return distance(p,vec3(0.0)) - 0.5;
}

/**********************
	0	background
	1	sphere
**********************/
int GetSurfaceID(vec3 pos)
{
	float dis = sdSphere(pos);
	
	if(dis<0.001)
		return 1;
	else
		return 0;
}

float map(vec3 pos)
{
	return sdSphere(pos);
}

vec3 Shade(vec3 ro, vec3 rd, float t)
{
	int id = GetSurfaceID(ro+rd*t);

	if(id == 1)
		return vec3(0.5);
	if(id == 0)
		return vec3(0.0);
}

float Intersect(vec3 ro, vec3 rd)
{
	float t = 0.0;
	for(int i =0; i < 128; ++i)
	{
		float dis = map(ro + t * rd);	
		if(dis < 0.001)	return t;
		t += dis;
	}

	return 	t;
}

vec3 Camera()
{
	vec2 p = (-iResolution.xy + 2.0*gl_FragCoord.xy) / iResolution.y;
	
	// camera anim
    vec3 ro = 3.0*vec3( 0.0, 0.0, 3.0);
	vec3 ta = vec3(0.0);

    // camera matrix	
	vec3  cw = normalize( ta-ro );							 	// forward ray
	vec3  cu = normalize( cross(cw,vec3(0.0,1.0,0.0)) );		// x
	vec3  cv = normalize( cross(cu,cw) );						// y
	vec3  rd = normalize( p.x*cu + p.y*cv + 1.7*cw );

	float t = Intersect(ro,rd);
	return Shade(ro,rd,t);
}

void main()
{
	vec3 col = Camera();	
    fragColor=vec4(col,1.0);
}
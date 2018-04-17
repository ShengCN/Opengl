#version 430
layout(location = 1) uniform float iTime;
layout(location = 2) uniform vec2 iResolution;
layout(location = 3) uniform vec2 iMouse;
layout(location = 4)uniform vec4 fcolor;

uniform sampler2D volume_tex;
uniform sampler2D noise_tex;
uniform int slice;
uniform int keyboard;
uniform vec4 slider;

out vec4 fragColor;

const vec3 LightPos = vec3(2.0,1.5,0.0)*10.0; 
const vec3 WaterColor = vec3(0.4, 0.9, 1);
const float waterSZ = 1.0;
const float waterHeight = 0.8;
const float MaxWaveAmplitude = 0.04;
// ***********************
// Data Structures
// ***********************
struct Intersection
{
	float dist;			// distance to the object
	int id;				// object id
};

struct MaterialInfo
{
	vec3 Kd;
	float Shininess;
};

// ***********************
// Functions
// ***********************
Intersection Intersect(vec3 ro, vec3 rd,float minD, float maxD);

// ***********************
//	Tool functions
// ***********************
mat3 rotate3X(float v)
{
    float s = sin(v);
    float c = cos(v);
    return mat3(
        1, 0, 0,
        0, c,-s,
        0, s, c
	);
}

float CyclicTime()
{
	return mod(iTime, 30.);
}

// ***********************
//	SD functions
// ***********************
float WaveAmplitude() 
{
	return MaxWaveAmplitude * exp(-CyclicTime() / 10.);
}

float WaterWave(vec3 a) 
{
	return WaveAmplitude() * sin((2. * a.x * a.x + 2. * a.z * a.z) - 10. * CyclicTime());
}

float sdWaterSurface(vec3 pos)
{
	vec3 sz = vec3(waterSZ,waterHeight,waterSZ);
	return length(max(abs(pos+vec3(0.0,WaterWave(pos),0.0)) - sz, 0.));
}

float sdSphere(vec3 pos)
{
	return length(pos-vec3(0.0,0.8,0.0)) - 0.5;
}

bool IsWater(vec3 pos)
{
	return (pos.y < (waterHeight-MaxWaveAmplitude));
}

// map is a function that maps a pos to the scene information
// Intersection has information:
// 1. intersect object id
// 2. distance to the instersect object
Intersection map(vec3 pos)
{
	Intersection result = Intersection(1000.0,1); 	// default is sky
	
	// find the closest intersect point
	float dis_water = sdWaterSurface(pos);
	if(result.dist > dis_water)
	{
		result.dist = dis_water;
		result.id = 1;
	}
	
	float dis_sphere = sdSphere(pos);
	if(result.dist>dis_sphere)
	{
		result.dist = dis_sphere;
		result.id = 2;
	}

	// calculate the closest object's normal

	return result;
}

/**********************
	Lighting and colors
**********************/
MaterialInfo Material(vec3 pos) {
	MaterialInfo m;

	Intersection itsct = map(pos);

	if(itsct.id == 1) 				// water
	{
		m.Kd = WaterColor;
		m.Shininess = 120.;
	}

	if(itsct.id == 2)				// sphere
	{
		m.Kd = vec3(0.1);
		m.Shininess = 10.;
	}

	return m;
}

float Occlusion(vec3 at, vec3 normal)
{
	float b = 0.;
	for (int i = 1; i <= 4; ++i) 
	{
		float L = .06 * float(i);
		Intersection d = map(at + normal * L);		
		b += max(0., L - d.dist);
	}
	return min(b, 1.);
}

vec3 Normal(vec3 pos)
{
	float epison = 0.00001;
	return normalize(vec3(map(pos+vec3(epison,0.0,0.0)).dist-map(pos-vec3(epison,0.0,0.0)).dist,
								 map(pos+vec3(0.0,epison,0.0)).dist-map(pos-vec3(0.0,epison,0.0)).dist,
								 map(pos+vec3(0.0,0.0,epison)).dist-map(pos-vec3(0.0,0.0,epison)).dist));

}

vec3 Lighting(vec3 pos, vec3 normal, vec3 eye, MaterialInfo m, vec3 lightColor, vec3 lightPos)
{
	vec3 lightDir = lightPos - pos;
	vec3 nlightDir = normalize(lightDir);
	// Intersection t = Intersect(pos,nlightDir, 0.001, 1000.0);
	// if(t.dist<length(lightDir))
	// {
	// 	//vec3 p = pos + nlightDir * t.dist;
	// 	return vec3(0.0);
	// }

	vec3 color = m.Kd * lightColor * max(0.0,dot(normal,nlightDir));
	if(m.Shininess>0.0)
	{
		vec3 h = normalize(nlightDir + normalize(eye-pos));
		color += lightColor * pow(max(0.0,dot(normal,h)),m.Shininess) * (m.Shininess + 8.0) / 25.0;
	}

	return color;
}

/**********************
	0	background
	1	water
	2   sphere     ---- for debug
**********************/
vec3 Shade(vec3 ro, vec3 rd, Intersection t)
{
	int id = t.id;
	// normal calculate
	vec3 pos = ro + t.dist * rd;
	vec3 nor = Normal(pos);

	return nor;

	if(id == 0)					// sky
	{
		return vec3(0.8);
	}
	if(id == 1)					// water
	{
		vec3 waterSurfaceLight = vec3(0.0);
		
		if(!IsWater(pos)) 		// water surface
		{
			// refraction
			waterSurfaceLight = Lighting(pos,nor,ro,Material(pos),vec3(1.0),LightPos);
			vec3 refractionDir = refract(rd,nor,0.9);
			Intersection inWaterIntersect = Intersect(pos,refractionDir,0.0,1000.0); 
			
			// update new pos and nor
			pos += refractionDir * inWaterIntersect.dist;
			nor = Normal(pos);
		}

		// otherwise, just water
		MaterialInfo mat = Material(pos);
		vec3 color = 0.11 * (1.0 - Occlusion(pos, nor)) * mat.Kd;
		color += Lighting(pos,nor,ro,mat,vec3(1.0),LightPos);
		color *= WaterColor;

		if(!IsWater(pos)) 		// water surface
		{
			// color += waterSurfaceLight;
		}
		return color;
	}
	if(id == 2)
	{
		MaterialInfo mat = Material(pos);
		vec3 color = .11 * (1. - Occlusion(pos, nor)) * mat.Kd;
		color += Lighting(pos,nor,ro,mat,vec3(1.0),LightPos);
		return color;
	}
}

Intersection Intersect(vec3 ro, vec3 rd,float minD, float maxD)
{
	Intersection t;
	ro += minD * rd;

	for(int i =0; i < 128; ++i)
	{
		Intersection curIntersect = map(ro + t.dist * rd);	
		if(curIntersect.dist < 0.00001)	return curIntersect;
		t.dist += curIntersect.dist;
	}

	// nothing hit
	return 	t;
}

vec3 Camera()
{
	vec2 p = (-iResolution.xy + 2.0*gl_FragCoord.xy) / iResolution.y;
	
	// camera anim
    vec3 ro = vec3( 0.0, 2.0, 3.0);
	vec3 ta = vec3(0.0);

    // camera matrix	
	vec3  cw = normalize( ta-ro );							 	// forward ray
	vec3  cu = normalize( cross(cw,vec3(0.0,1.0,0.0)) );		// x
	vec3  cv = normalize( cross(cu,cw) );						// y
	vec3  rd = normalize( p.x*cu + p.y*cv + 2.0 *cw);

	Intersection t = Intersect(ro,rd,0.0, 1000.0);
	return Shade(ro,rd,t);
}

void main()
{
	vec3 col = Camera();	
	col = pow(col,vec3(1.0/2.2));
	// col += texture(noise_tex,gl_FragCoord.xy/256.0).xyz/100.0;
    fragColor=vec4(col,1.0);
}
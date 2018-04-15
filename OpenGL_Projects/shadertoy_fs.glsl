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

// ***********************
//	SD functions
// ***********************
float sdWater(vec3 pos)
{
	float height = -2.0;
	return pos.y - height;
}

// ***********************
// Lighting functions
// ***********************
vec3 palette(vec3 a, vec3 b, vec3 c, float t)
{
    float x = smoothstep(0.0, 0.7, t);
    float y = smoothstep(0.5, 1.0, t);
    return mix(a, mix(c, b, y), x);
}

vec3 sun(vec3 ro, vec3 rd)
{
	vec3 dir = normalize(vec3(-0.04, -0.26, 0.4));
	float n = dot(normalize(rd), dir) * 0.5 + 0.62 - rd.y * 0.3;
	n = pow(n,16.0);
	return palette(vec3(0.003, 0.003, 0.050), vec3(0.95, 0.85, 0.8), vec3(1.2, 0.5, 0.1), n);
}

vec3 stars(vec3 ro,vec3 rd)
{
    vec3 f = texture(noise_tex, rd.xy * 2.0).xyz;
    float p = length(pow(f, vec3(100.0, 75.0, 50.0)) * vec3(0.05, 0.02, 0.01));
    return vec3(p);
}


vec3 venus(vec3 ro, vec3 rd)
{
	vec2 n = vec2(0.5,0.1) - rd.xy;
	float t = length(dFdx(n));
    float v = smoothstep(t * 2.2, t * 0.2, length(n));
    return v * vec3(0.17, 0.16, 0.15);
}

vec4 clouds(vec3 ro, vec3 rd)
{
	float dist = (ro.y + 2000.0)/rd.y;
	vec3 pos = ro + dist * rd;
	pos.z += sin(pos.x * 0.0005 + 2.0) * 500.0;
	float a = max(0.0, sin(pos.z * 0.003 + pos.x * 0.001) * 0.5 + 0.6);
    float m = smoothstep(13000.0, 5000.0, distance(pos.xz, vec2(-15000, 10000)));
    float t = a * m * 0.4;
    return vec4(mix(vec3(0.0), vec3(0.20, 0.07, 0.01) * 0.4, t), t);
}

vec3 sky(vec3 ro, vec3 rd)
{
	vec3 power = vec3(1.0);
	vec4 c = clouds(ro,rd);
	c.xyz = pow(c.xyz, power);
	vec3 s = pow(sun(ro,rd), power) + venus(ro,rd) + stars(ro,rd);
	return mix(s,c.xyz,c.w);
}

vec2 trees(vec3 ro, vec3 rd)
{
	vec2 result = vec2(0.0);
	result.x = (ro.z - 200.0) / -rd.z;
	vec3 pos = ro + result.x * rd;
	float n = pos.y;

	n *= (smoothstep(0.0, 600.0, abs(pos.x + 30.0)) + 0.18);
    n *= (smoothstep(0.0, 200.0, abs(pos.x + 300.0)));
    n += sin(pos.x + pos.y * 2.0) * 0.05;
    n += (texture(noise_tex, vec2(pos.x * 0.004, pos.x * 0.003)).x - 0.5) * 0.5;
    float t = max(0.1, length(vec2(dFdx(n), dFdy(n))));
    result.y = smoothstep(4.0 + t, 4.0 - t, n);
    return result;
}

struct Intersection
{
	float dist;			// distance to the object
	int id;				// object id
};


// map is a function that maps a pos to the scene information
// Intersection has information:
// 1. intersect object id
// 2. distance to the instersect object
Intersection map(vec3 pos)
{
	Intersection result;
	
	// find the closest intersect point
	float dis_water = sdWater(pos);
	result.dist = dis_water;
	result.id = 1;
	// .....

	// find the closest
	// if(...)

	// calculate the closest object's normal

	return result;
}

/**********************
	0	background
	1	water
**********************/
vec3 Shade(vec3 ro, vec3 rd, Intersection t)
{
	int id = t.id;
	// normal calculate
	float epison = 0.001;
	vec3 pos = ro + t.dist * rd;
	vec3 normal = normalize(vec3(map(pos+vec3(epison,0.0,0.0)).dist-map(pos-vec3(epison,0.0,0.0)).dist,
								 map(pos+vec3(0.0,epison,0.0)).dist-map(pos-vec3(0.0,epison,0.0)).dist,
								 map(pos+vec3(0.0,0.0,epison)).dist-map(pos-vec3(0.0,0.0,epison)).dist));

	if(id == 0)					// sky
	{
		vec2 tree = trees(ro,rd);
		vec3 s = sky(ro,rd);
		float b = 1.0;
		return mix(s, vec3(0, 0, 0), tree.y) * b;
	}
	if(id == 1)					// water
	{
		// fake normal
		normal = normalize(vec3(sin(pos.z + iTime) * 0.2,
        				  		10.0 + (pos.z + 15.0) * 0.33,
        						cos(pos.x + iTime * 0.1) * 0.05));

		float b = 1.0;
		vec2 tree = trees(ro,rd);

		if(t.dist>0.0 && t.dist<tree.x)
		{
			b = pow(1.1 - abs(dot(normal, rd)), 5.0);
			// ro = min(pos,199.9);			
			rd = reflect(rd,normal);
			tree = trees(ro,rd);
		}
		vec3 s = sky(ro,rd);
		return mix(s, vec3(0, 0, 0), tree.y) * b;;
	}
}

Intersection Intersect(vec3 ro, vec3 rd)
{
	Intersection t;
	for(int i =0; i < 128; ++i)
	{
		Intersection curIntersect = map(ro + t.dist * rd);	
		if(curIntersect.dist < 0.001)	return curIntersect;
		t.dist += curIntersect.dist;
	}

	// nothing hit
	return 	t;
}

vec3 Camera()
{
	vec2 p = (-iResolution.xy + 2.0*gl_FragCoord.xy) / iResolution.y;
	
	// camera anim
    vec3 ro = vec3( 0.0, 0.0, -3.0);
	vec3 ta = vec3(0.0) + slider.xyz;

    // camera matrix	
	vec3  cw = normalize( ta-ro );							 	// forward ray
	vec3  cu = normalize( cross(cw,vec3(0.0,1.0,0.0)) );		// x
	vec3  cv = normalize( cross(cu,cw) );						// y
	vec3  rd = normalize( p.x*cu + p.y*cv + 2.0 *cw);

	Intersection t = Intersect(ro,rd);
	return Shade(ro,rd,t);
}

void main()
{
	vec3 col = Camera();	
	col = pow(col,vec3(1.0/2.2));
	// col += texture(noise_tex,gl_FragCoord.xy/256.0).xyz/100.0;
    fragColor=vec4(col,1.0);
}
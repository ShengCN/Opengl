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
uniform float angle;
uniform int octave;

out vec4 fragColor;

const vec3 LightPos = vec3(7.0,6.5,1.4) *0.1; 
const vec3 WaterColor = vec3(0.4, 0.9, 1);
const float waterSZ = 1.0;
const float waterHeight = 0.8;
const float MaxWaveAmplitude = 0.04;
const float SC = 25.0;
const mat2 m2 = mat2(0.8,-0.6,0.6,0.8);

// ***********************
// Data Structures
// ***********************
struct MaterialInfo
{
	vec3 Kd;
	float Shininess;
};

// ***********************
// Functions
// ***********************
vec2 Intersect(vec3 ro, vec3 rd,float tmin, float tmax);
vec2 map(vec3 pos);

// ***********************
//	Tool functions
// ***********************
float CyclicTime()
{
	return mod(iTime, 30.);
}

vec3 calcNormal(vec3 p)
{
    vec3 e = vec3(0.001,0.0,0.0);
    vec3 n;
    n.x = map(p+e.xyy).x - map(p-e.xyy).x;
    n.y = map(p+e.yxy).x - map(p-e.yxy).x;
    n.z = map(p+e.yyx).x - map(p-e.yyx).x;
    return normalize(n);
}

vec3 noised(in vec2 x)
{
	vec2 f = fract(x);
	vec2 u = f*f*(3.0-2.0*f);

	ivec2 p = ivec2(floor(x));
	float a = texelFetch(noise_tex,(p+ivec2(0,0))&255,0).x;
	float b = texelFetch(noise_tex,(p+ivec2(1,0))&255,0).x;
	float c = texelFetch(noise_tex,(p+ivec2(0,1))&255,0).x;
	float d = texelFetch(noise_tex,(p+ivec2(1,1))&255,0).x;

	return vec3(a+(b-a)*u.x+(c-a)*u.y+(a-b-c+d)*u.x*u.y,
			6.0*f*(1.0-f)*(vec2(b-a,c-a)+(a-b-c+d)*u.yx));
}

// ***********************
//	SD functions
// ***********************
float terrainL(in vec2 x)
{
	vec2  p = (x+slider.yz)*3.0;
    float a = 0.0;
    float b = 1.0;
	vec2  d = vec2(1.0);
    for( int i=0; i<1+octave; i++ )
    {
        vec3 n = noised(p);
        d += n.yz;
        a += b*n.x/(1.0+dot(d,d));
		b *= 0.5;
        p = m2*p*2.0;
    }

	return a*5.0;
}

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
	pos.y -= 0.8;
	vec3 sz = vec3(waterSZ,0.0,waterSZ);
	return length(max(abs(pos+vec3(0.0,WaterWave(pos),0.0)) - sz, 0.));
}

float sdSphere(vec3 pos)
{
	return length(pos-vec3(0.0,0.0,0.0)) - 0.5;
}

float sdPlane(vec3 pos)
{
	vec4 plane = vec4(0.0,1.0,0.0,0.0);
	//vec3 nor = noised(pos.xz);
	// return dot(pos,nor) + 0.5;
	return (pos.y - terrainL(pos.xz));
}



float sdTerrain(in vec3 pos)
{
	float h = terrainL(pos.xz);
	return 0.5 * (pos.y - h);
}

bool IsWater(vec3 pos)
{
	return (pos.y < (waterHeight-MaxWaveAmplitude));
}

bool FloatEqual(float a, float b)
{
	return abs(a-b)<0.1;
}

/**********************
	Lighting and colors
**********************/
vec2 softShadow(in vec3 ro, in vec3 rd)
{
	vec2 res = vec2(1.0,0.0);
	for(float t = 0.01; t < 150.0;)
	{
		vec2 h = map(ro+t*rd);
		if(h.x<0.0001) return vec2(0.0,h.y);
        if(!FloatEqual(h.y,3.0)) res.x = min(res.x,6.0 * h.x / t);
        t += h.x;
	}
	return res;
}

vec2 refractIntersect(in vec3 ro, in vec3 rd)
{
	for(float t = 0.01; t < 15.0;)
	{
		vec2 h = map(ro+t*rd);
		if(h.x < 0.0001) return vec2(t,h.y);
		t += h.x;
	}	
	return vec2(0.0);
}

vec3 Lighting(vec3 pos, vec3 normal, vec3 eye, MaterialInfo m, vec3 lightColor, vec3 lightPos)
{
	vec3 lightDir = lightPos - pos;
	vec3 nlightDir = normalize(lightDir);
	vec3 light = normalize(vec3(1.0,0.8,0.6));
	// light = nlightDir;
	vec3 viewDir = normalize(pos-eye);
	vec3 ref = reflect(viewDir,normal);

	// Phong light
	float amb = 0.6 + 0.5 * normal.y;
	float dif = max(0.0,dot(normal,light));
	float spe = pow(max(dot(light,ref),0.0),m.Shininess) * (m.Shininess + 8.) / 25.0;
	vec2 sha = softShadow(pos,light);
	if(FloatEqual(sha.y,3.0)) sha.x = 1.0;

	vec3 color = amb * vec3(0.3);
	color += dif * sha.x * m.Kd * lightColor;
	if(spe>0.0) color += spe * lightColor * 0.08;

	// return color / dot(lightDir,lightDir);
	return color;
}

/**********************
	0	background
	1	ground
	2   sphere     ---- for debug
**********************/
MaterialInfo Material(vec3 pos) {
	MaterialInfo m;

	vec2 itsct = map(pos);

	if(FloatEqual(itsct.y,1.0)) 			// plane
	{
		m.Kd = vec3(0.0);
		m.Shininess = 0.0;
	}

	if(FloatEqual(itsct.y,2.0))				// sphere
	{
		m.Kd = vec3(0.1);
		m.Shininess = 10.0;
	}

	if(FloatEqual(itsct.y,3.0))				// water
	{
		m.Kd = WaterColor;
		m.Shininess = 120.0;
	}

	return m;
}

vec3 Shade(vec3 ro, vec3 rd, vec2 t)
{
	float id = t.y;
	// normal calculate
	vec3 pos = ro + t.x * rd;
	vec3 nor = calcNormal(pos);
	MaterialInfo m = Material(pos);

	vec3 color 	= vec3(0.0);

	if(FloatEqual(id,0))					// sky
	{
		color = vec3(0.0);
	}
	if(FloatEqual(id,1.0))					// ground
	{
		color = Lighting(pos,nor,ro,m,vec3(1.0),LightPos);
	}
	if(FloatEqual(id,2.0))					// sphere
	{
		color = Lighting(pos,nor,ro,m,vec3(1.0),LightPos);
	}
	if(FloatEqual(id,3.0))					// water
	{
		vec3 surfaceColor = Lighting(pos,nor,ro,m,vec3(1.0),LightPos);
		bool isSurface = !IsWater(pos); 

		// refract
		vec3 refractionDir = refract(normalize(rd),nor,0.9);
		vec2 refractTrace = refractIntersect(pos,refractionDir); 
		pos += refractionDir * refractTrace.x;
		nor = calcNormal(pos);
		m = Material(pos);

		color = Lighting(pos,nor,ro,m,vec3(1.0),LightPos);
		color *= WaterColor;
		if(isSurface)	color+= surfaceColor;
	}

	return color;
}

vec2 map(vec3 pos)
{
	vec2 d1 = vec2(sdPlane(pos),1.0);
	vec2 d2 = vec2(sdSphere(pos),2.0);
	vec2 d3 = vec2(sdWaterSurface(pos),3.0);

	if(d2.x<d1.x) d1 = d2;
	if(d3.x<d1.x) d1 = d3;
	return d1;
}

vec2 Intersect(vec3 ro, vec3 rd,float tmin, float tmax)
{
	float t = tmin;
    for(int i = 0; i < 256; i++)
    {
		vec3 pos = ro + t * rd;
		vec2 h = map(pos);
		// deal with terrain
		// float te = pos.y - terrainL(pos.xz);

        if(h.x<0.0001) return vec2(t,h.y);
        t += h.x;
		
		if(t>tmax)	break;
    }

    return vec2(0.0);
}

vec3 Camera()
{
	vec2 p = (-iResolution.xy + 2.0*gl_FragCoord.xy) / iResolution.y;

	// camera anim
	float cr = 3.0;
	float cTime = angle * 3.1415 / 20.0;
    vec3 ro = vec3(cr * sin(cTime) , 2.0, cr * cos(cTime))*3.0;
	vec3 ta = vec3(0.0);

    // camera matrix	
	vec3  cw = normalize( ta-ro );							 	// forward ray
	vec3  cu = normalize( cross(cw,vec3(0.0,1.0,0.0)) );		// x
	vec3  cv = normalize( cross(cu,cw) );						// y
	vec3  rd = normalize( p.x*cu + p.y*cv + 2.0 *cw);

	vec2 t = Intersect(ro,rd,0.0,1000.0);
	return Shade(ro,rd,t);
}

void main()
{
	vec3 col = Camera();	
	//col = pow(col,vec3(1.0/2.2));
	// col += texture(noise_tex,gl_FragCoord.xy/256.0).xyz/100.0;
    fragColor=vec4(col,1.0);
}
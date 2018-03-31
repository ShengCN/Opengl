#version 430

layout(location = 1) uniform int pass;
layout(location = 2) uniform sampler2D backfaces_tex;
layout(location = 3) uniform int mode = 0;
layout(location = 6) uniform float time;
layout(location = 7) uniform vec4 slider;
layout(location = 8) uniform int scene = 0;
uniform bool isAntiAliasing;

layout(location = 0) out vec4 fragcolor;

in vec3 vpos;

float PI = 3.1415926;
mat4 rotationMatrix(vec3 axis, float angle)
{
	axis = normalize(axis);
	float s = sin(angle);
	float c = cos(angle);
	float oc = 1.0 - c;

	return mat4(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0.0,
		oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0.0,
		oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0.0,
		0.0, 0.0, 0.0, 1.0);
}

float opS(float d1, float d2)
{
	return max(-d1, d2);
}

//forward function declarations
vec4 raytracedcolor(vec3 rayStart, vec3 rayStop);
vec4 lighting(vec3 pos, vec3 rayDir);
float distToShape(vec3 pos);
vec3 normal(vec3 pos);
float softShadow(in vec3 ro, in vec3 rd, float mint, float maxt, float k);
float calcAO(in vec3 pos, in vec3 nor);

// sd functions
//shape function declarations
float sdSphere(vec3 p, float s);
float DE_julia(vec3 p);
float sdCappedCylinder(vec3 p, vec2 h);
float sdPlane(vec3 p, vec4 n);

void main(void)
{
	if (pass == 1)
	{
		fragcolor = vec4((vpos), 1.0); //write cube positions to texture
	}
	else if (pass == 2)
	{
		if (mode == 0) // for debugging: show backface colors
		{
			fragcolor = texelFetch(backfaces_tex, ivec2(gl_FragCoord), 0);
			return;
		}
		else if (mode == 1) // for debugging: show frontface colors
		{
			fragcolor = vec4((vpos), 1.0);
			return;
		}
		else // raycast
		{
			vec3 rayStart = vpos.xyz;
			vec3 rayStop = texelFetch(backfaces_tex, ivec2(gl_FragCoord.xy), 0).xyz;
			
			if(!isAntiAliasing)
			{
				fragcolor = raytracedcolor(rayStart, rayStop);
				return;
			}
			
			// Anti-aliasing
			int ns = 4;
			int samples = ns * ns;
			vec4 col = vec4(0.0);
			for(float i = -(ns-1.0)/2; i <= (ns-1.0)/2; ++i)
			{
				for(float j = -(ns-1.0)/2; j <= (ns-1.0)/2;++j)
				{
					vec3 offsetX = dFdx(rayStop)/float(ns)*i;
					vec3 offsetY = dFdy(rayStop)/float(ns)*j;
					col += raytracedcolor(rayStart,rayStop+offsetX+offsetY)/float(samples);
				}
			}
			fragcolor = col;
			
			
			if (fragcolor.a == 0.0) discard;
		}
	}
}

// trace rays until they intersect the surface
vec4 raytracedcolor(vec3 rayStart, vec3 rayStop)
{
	vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
	const int MaxSamples = 1000;

	vec3 rayDir = normalize(rayStop - rayStart);
	float travel = distance(rayStop, rayStart);
	float stepSize = travel / MaxSamples;
	vec3 pos = rayStart;
	vec3 step = rayDir*stepSize;

	for (int i = 0; i < MaxSamples && travel > 0.0; ++i, pos += step, travel -= stepSize)
	{
		float dist = distToShape(pos);

		stepSize = dist;
		step = rayDir*stepSize;

		if (dist <= 0.001)
		{
			color = lighting(pos, rayDir);
			return color;
		}
	}

	// sky
	color = rayDir.x * vec4(1.0);
	return color;
}

//Compute lighting on the raycast surface using Phong lighting model
vec4 lighting(vec3 pos, vec3 rayDir)
{
	const vec3 light = vec3(0.577, 0.577, 0.577); 			//light direction
	float softShadow_effect = softShadow(pos, light, 0.01, 6.5, 16);  // compute softShadows

	const vec4 ambient_color = vec4(0.071, 0.188, 0.294, 1.0);
	const vec4 diffuse_color = vec4(0.722, 0.627, 0.545, 1.0);
	const vec4 spec_color = vec4(0.120, 0.120, 0.060, 1.0);

	vec3 n = normal(pos);
	vec3 v = -rayDir;
	vec3 r = reflect(-light, n);

	float ao_effect = calcAO(pos, n);
	return ambient_color*ao_effect + (diffuse_color*max(0.0, dot(n, light)) + spec_color*pow(max(0.0, dot(r, v)), 15.0) * ao_effect)*softShadow_effect;
}

// For more distance functions see
// http://iquilezles.org/www/articles/distfunctions/distfunctions.htm

// Soft softShadows
// http://www.iquilezles.org/www/articles/rmsoftShadows/rmsoftShadows.htm

// WebGL example and a simple ambient occlusion approximation
// https://www.shadertoy.com/view/Xds3zN


//distance to the shape we are drawing
float distToShape(vec3 pos)
{
	const vec4 ground = normalize(vec4(0.0, 0.0, 1.0, 2.0));
	float res = sdPlane(pos, ground);
	if (scene == 0)
	{
		const float radius = 0.4;
		res = min(res, sdSphere(pos, radius));
	}

	if (scene == 1)
	{
		res = min(res, (1.0 / 1.2)*DE_julia(1.2*pos));
	}

	if (scene == 2)
	{
		//return (1.0/1.2)*DE_julia(1.2*pos)*0.5*(1.0+sin(time*0.1));
		vec2 c1 = vec2(0.5, 0.75);
		vec2 c2 = vec2(0.58, 0.55) + slider.xy;
		vec4 q = rotationMatrix(vec3(1.0, 0.0, 0.0), 0.35*PI + slider.z) * vec4(pos, 1.0);
		vec4 ground = normalize(vec4(vec3(0.0, 0.0, 1.0), slider.w));
		res = min(res, opS(sdCappedCylinder(q.xyz, c1), sdCappedCylinder(q.xyz, c2)));
		//return sdCappedCylinder(q.xyz,c2);
	}
	return res;
}


// shape function definitions

float sdSphere(vec3 p, float s)
{
	//float c = slider.x * 0.1;
	float c = 1.5;
	vec3 q = mod(p, vec3(c, c, 0.0)) - 0.5 * vec3(c);
	return length(q) - s;
}

float DE_julia(vec3 pos)
{
	const vec4 c = 0.8 * vec4(sin(time), cos(time), 0.0, 0.0);
	vec4 z = vec4(pos, 0.0);
	vec4 nz;
	float md2 = 1.0;
	float mz2 = dot(z, z);

	for (int i = 0; i<18; i++)
	{
		// |dz|^2 -> 4*|dz|^2
		md2 *= 4.0*mz2;
		// z -> z2 + c
		nz.x = z.x*z.x - dot(z.yzw, z.yzw);
		nz.yzw = 2.0*z.x*z.yzw;
		z = nz + c;
		mz2 = dot(z, z);

		if (mz2 > 22.0) //Bailout
		{
			break;
		}
	}
	return 0.25*sqrt(mz2 / md2)*log(mz2);
}

float sdCappedCylinder(vec3 p, vec2 h)
{
	vec2 d = abs(vec2(length(p.xz), p.y)) - h;
	return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

float sdPlane(vec3 p, vec4 n)
{
	return dot(p, n.xyz) + n.w;
}

//normal vector of the shape we are drawing.
//Estimated as the gradient of the signed distance function.
vec3 normal(vec3 pos)
{
	const float h = 0.001;
	const vec3 Xh = vec3(h, 0.0, 0.0);
	const vec3 Yh = vec3(0.0, h, 0.0);
	const vec3 Zh = vec3(0.0, 0.0, h);

	return normalize(vec3(distToShape(pos + Xh) - distToShape(pos - Xh), distToShape(pos + Yh) - distToShape(pos - Yh), distToShape(pos + Zh) - distToShape(pos - Zh)));
}

float softShadow(in vec3 ro, in vec3 rd, float mint, float maxt, float k)
{
	float res = 1.0;
	for (float t = mint; t < maxt; )
	{
		float h = distToShape(ro + rd*t);
		if (h<0.001)
			return 0.0;
		res = min(res, k*h / t);
		t += h;
	}
	return res;
}

float calcAO(in vec3 pos, in vec3 nor)
{
	float occ = 0.0;
	float sca = 1.0;
	for (int i = 0; i < 5; i++)
	{
		float hr = 0.01 + 0.12 * float(i) / 4.0;
		vec3 aopos = nor * hr + pos;
		float dd = distToShape(aopos).x;
		occ += -(dd - hr)*sca;
		sca *= 0.95;
	}
	return clamp(1.0 - 3.0 * occ, 0.0, 1.0);
}
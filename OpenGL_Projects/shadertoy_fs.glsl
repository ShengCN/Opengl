#version 430
layout(location = 1) uniform float iTime;
layout(location = 2) uniform vec2 iResolution;
layout(location = 3) uniform vec2 iMouse;
layout(location = 4)uniform vec4 fcolor;

uniform vec4 camera;
uniform vec4 slider;
uniform float angle;
uniform float globalTime;
uniform vec3 camera_pos;
uniform samplerCube cubemap;

out vec4 fragColor;

float PI = 3.1415926;

struct PPC{
	vec3 C;
	vec3 a;
	vec3 b;
	vec3 c;
};

struct Ray{
	vec3 ro;
	vec3 rd;
};

float Deg2Rad(float deg)
{
	return deg / 180.0 * PI;
}

mat3 SetRotate(int axis, float deg)
{
	mat3 ret;
	float rad = Deg2Rad(deg);
	if(axis == 1)
	{
		ret[0] = vec3(1.0f, 0.0f, 0.0f);
		ret[1] = vec3(0.0f, cos(rad), -sin(rad));
		ret[2] = vec3(0.0f, sin(rad), cos(rad));
	}
	else if(axis ==2 )
	{
		ret[0] = vec3(cos(rad),0.0f, sin(rad));
		ret[1] = vec3(0.0f, 1.0f, 0.0f);
		ret[2] = vec3(-sin(rad), 0.0f, cos(rad));
	}
	else if(axis ==3)
	{
		ret[0] = vec3(cos(rad), -sin(rad), 0.0f);
		ret[1] = vec3(sin(rad), cos(rad), 0.0f);
		ret[2] = vec3(0.0f, 0.0f, 1.0f);
	}
	return ret;
}

void LootAt(in vec3 ro, in vec3 lat, in vec3 up, out vec3 a, out vec3 b)
{
	vec3 rd = normalize(lat-ro);
	a = cross(rd, up);
	b = cross(rd, a);
}

Ray GetRay(in vec3 ro, in vec3 lat, in vec3 up)
{
	vec3 a,b;
	LootAt(ro, lat, up, a, b);

	vec3 rd = cross(a,b);
	vec2 uv = (2.0 * gl_FragCoord.xy / iResolution - 1.0) * vec2(iResolution.x/iResolution.y,-1.0);

	Ray ret;
	ret.ro = ro;
	ret.rd = normalize(a * uv.x + b * uv.y + rd);

	return ret;
}

vec3 C = vec3(0.0, 0.0,-5.0);
float radius = 1.0;
float sdSphere(Ray r)
{
	vec3 ro = r.ro;
	vec3 rd = r.rd;
	// |xyz|^2 = r^2
	// |xyz| = |p-C| = ro - C + rd * t
	// rd^2 * t^2 + 2 * rd * (ro - C) * t + (ro - C)^2 - r^2 = 0
	float a = dot(rd, rd);
	float b = 2.0 * dot(rd, ro - C);
	float c = dot(ro-C, ro-C) - radius*radius;
	float h = b * b - 4.0 * c;
	if(h < 0.0)	return -1.0;
	float t = (-b - sqrt(h))/(2.0*a);
	return t;
}

vec3 TraceScene(Ray r)
{
	vec3 col = vec3(0.0);
	// env mapping
	col = texture(cubemap, r.rd).rgb;

	// find the closest hit point
	float t = sdSphere(r);

	if(t>0.0)
		col = vec3(0.5);

	return col;
}

void main()
{
	vec3 cameraC = vec3(0.0);
	vec3 target = vec3(0.0, 0.0, -1.0);
	mat3 rotMat = SetRotate(2,angle/20.0 * 360.0);
	target = rotMat * target;

	Ray r = GetRay(cameraC, target, vec3(0.0,1.0,0.0));

	vec3 col = vec3(0.0);
	col = TraceScene(r);
    // fragColor=vec4(col,1.0);
	fragColor=vec4(col,1.0);
}
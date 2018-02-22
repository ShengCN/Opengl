#version 430            

uniform mat4 PVM;
uniform float time;
uniform float slider;

layout(location = 0) in vec3 pos_attrib;
layout(location = 1) in vec3 vel_attrib;
layout(location = 2) in float age_attrib;

out vec3 pos_out; 
out vec3 vel_out; 
out float age_out;
out vec3 camera_pos;

vec3 vel(vec3 p);
float rand(vec2 co);
vec3 vec_func(vec3 p);
vec3 v0(vec3 p);
vec3 v(vec3 p);

float sdSphere(vec3 p)
{
    return length(p) - 6.0;
}

float sdPlane(vec3 p)
{
    return p.y + 5.0;
}

vec3 sphereNormal(vec3 p)
{
    float e = 1e-3;
    vec3 dx = vec3(e,0.0,0.0);
    vec3 dy = vec3(0.0,e,0.0);
    vec3 dz = vec3(0.0,0.0,e);

    float dxde = (sdSphere(p+dx) - sdSphere(p-dx))/(2*e);
    float dyde = (sdSphere(p+dy) - sdSphere(p-dy))/(2*e);
    float dzde = (sdSphere(p+dz) - sdSphere(p-dz))/(2*e);

    return vec3(dxde,dyde,dzde);
}

vec3 planeNormal(vec3 p)
{
    float e = 1e-3;
    vec3 dx = vec3(e,0.0,0.0);
    vec3 dy = vec3(0.0,e,0.0);
    vec3 dz = vec3(0.0,0.0,e);

    float dxde = (sdPlane(p+dx) - sdPlane(p-dx))/(2*e);
    float dyde = (sdPlane(p+dy) - sdPlane(p-dy))/(2*e);
    float dzde = (sdPlane(p+dz) - sdPlane(p-dz))/(2*e);

    return vec3(dxde,dyde,dzde);
}


void main()
{
    gl_Position = PVM * vec4(pos_attrib,1.0);
    gl_PointSize = mix(1.0,10.0, age_attrib/1000.0);

    vel_out = vec_func(pos_attrib);
    pos_out = pos_attrib + 0.05 * vel_out;
    age_out = age_attrib - 1.0;

    if(age_out <= 0.0 || length(pos_out) > 10.0f)
    {
        vec2 seed = vec2(float(gl_VertexID),time);
        pos_out = pos_attrib*0.0003 + (vec3(rand(seed.xy), rand(seed.yx), rand(seed.xx)) - vec3(0.5)) + vec3(0.0,6.0,0.0);
        age_out = 500.0 + 200.0*(rand(pos_attrib.xy+seed)-0.5);
    }

}

vec3 vec_func(vec3 p)
{
    vec3 a = vec3(0.0,-1.0,0.0)*3.0;
    // if near surface
    if(sdSphere(p)<1e-6)
    {
        vec3 n = -sphereNormal(p);
        a = vel_attrib - n;
    }

    // if near ground
    if(sdPlane(p)<1e-6)
    {
        vec3 n = -planeNormal(p);
        //a = vel_attrib - n;
        a.y = 0.0;

        vec2 seed = vec2(float(gl_VertexID),time);
        a.x =  rand(seed.xy);
        a.z = rand(seed.yx);
    }

    
    
    
    return a;
}

vec3 vel(vec3 p)
{
    const int n = 8;
    vec3 octaves = vec3(0.0);
    float scale = 1.0;
    for(int i = 0; i < n; ++i)
    {
        octaves = octaves + v(scale*p)/scale;
        scale *= 2.0;
    }
    return octaves;
}

vec3 v0(vec3 p)
{
    // return 15.0*vec3(sin(p.y*10.0+time-10.0), -sin(p.x*10.0+9.0*time+10.0), +cos(2.4*p.z+2.0*time));
    return vec3(sin(p.y*5.0), -sin(p.x*5.0), +cos(1.2*p.z));
}

vec3 v(vec3 p)
{
	return v0(p.xyz + vec3(0.0, 0.5*time, 0.0)) + v0(p.zyx);
} 

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
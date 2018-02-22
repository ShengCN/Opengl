#version 430            

uniform mat4 PVM;
uniform float time;
uniform float slider;
uniform mat4 V;

layout(location = 0) in vec3 pos_attrib;
layout(location = 1) in vec3 vel_attrib;
layout(location = 2) in float age_attrib;


out vec3 pos_out; 
out vec3 vel_out; 
out float age_out;
out vec4 camera_pos;

vec3 vel(vec3 p);
float rand(vec2 co);
vec3 vec_func(vec3 p);
vec3 v0(vec3 p);
vec3 v(vec3 p);
vec3 sample_potential(vec3 p);

vec3 blend_vec(vec3 v1, vec3 v2, float alpha)
{
    return alpha* v1 + (1.0-alpha)*v2;
}

vec3 curl(vec3 p)
{
    const float e = 1e-4;
    vec3 dx = vec3(e,0.0,0.0);
    vec3 dy = vec3(0.0,e,0.0);
    vec3 dz = vec3(0.0,0.0,e);

    float x = sample_potential(p + dy)[2] - sample_potential(p - dy)[2]
            - sample_potential(p + dz)[1] + sample_potential(p - dz)[1];

    float y = sample_potential(p + dz)[0] - sample_potential(p - dz)[0]
            - sample_potential(p + dx)[2] + sample_potential(p - dx)[2];

    float z = sample_potential(p + dx)[1] - sample_potential(p - dx)[1]
            - sample_potential(p + dy)[0] + sample_potential(p - dy)[0];

    return vec3(x, y, z) / (2*e);
}

float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

float noise (in vec2 _st) {
    vec2 i = floor(_st);
    vec2 f = fract(_st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

#define NUM_OCTAVES 5
float fbm ( in vec2 _st) {
    float v = 0.0;
    float a = 0.5;
    vec2 shift = vec2(100.0);
    // Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.5), sin(0.5),
                    -sin(0.5), cos(0.50));
    for (int i = 0; i < NUM_OCTAVES; ++i) {
        v += a * noise(_st);
        _st = rot * _st * 2.0 + shift;
        a *= 0.5;
    }
    return v;
}


float sdSphere(vec3 p)
{
    return length(p+vec3(0.0,0.37,0.0)) - 0.7;
}

float sdPlane(vec3 p)
{
    return p.y + 0.5;
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

vec3 sdSphereGradient(vec3 p)
{
    const float e = 1e-2;
    vec3 dx = vec3(e,0.0,0.0);
    vec3 dy= vec3(0.0,e,0.0);
    vec3 dz= vec3(0.0,0.0,e);

    float d = sdSphere(p);
    float dfdx = sdSphere(p+dx) - d;
    float dfdy = sdSphere(p+dy) - d;
    float dfdz = sdSphere(p+dz) - d;

    return normalize(vec3(dfdx,dfdy,dfdz));
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
    age_out = age_attrib - 1.0*0.1;

    if(age_out <= 0.0 || length(pos_out) > 10.0f)
    {
        vec2 seed = vec2(float(gl_VertexID),time);
        pos_out = pos_attrib*0.0003 + (vec3(rand(seed.xy), rand(seed.yx), rand(seed.xx)) - vec3(0.5));
        age_out = 500.0 + 200.0*(rand(pos_attrib.xy+seed)-0.5);
    }

    camera_pos = inverse(V) * vec4(0.0,0.0,0.0,1.0);
}

vec3 vec_func(vec3 p)
{
    vec3 a = vec3(0.0,3.0,0.0);
    float scale = 5.0;

    float noiseX = fbm(p.xy) * scale;
    float noiseY = fbm(p.zy) * scale;
    float noiseZ = fbm(p.zx) * scale;

    vec3 noiseP = vec3(noiseX,noiseY,noiseZ);


    a = blend_vec(a,noiseP,0.1);

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

vec3 sample_potential(vec3 p)
{
    vec3 gradient = sdSphereGradient(p);

    float dis = sdSphere(p);

    return vec3(0.0);

}
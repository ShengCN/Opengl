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

vec3 vel(vec3 p);
float rand(vec2 co);

void main()
{
    gl_Position = PVM * vec4(pos_attrib,1.0);
    gl_PointSize = mix(4.0,24.0, age_attrib/1000.0);

    vel_out = vel(pos_attrib);
    pos_out = pos_attrib + 0.001 * vel_out;
    age_out = age_attrib - 1.0;

    if(age_out <= 0.0 || length(pos_out) > 2.0f)
    {
        vec2 seed = vec2(float(gl_VertexID),time);
        pos_out = pos_attrib*0.0003 + (vec3(rand(seed.xy), rand(seed.yx), rand(seed.xx)) - vec3(0.5));
        age_out = 500.0 + 200.0*(rand(pos_attrib.xy+seed)-0.5);
    }

}

vec3 v0(vec3 p);

vec3 vel(vec3 p)
{
    const int n = 6;
    vec3 octaves = vec3(0.0);
    float scale = 1.0;
    for(int i = 0; i < n; ++i)
    {
        octaves = octaves + v0(scale*p)/scale;
        scale *= 2.0;
    }
    return octaves;
}

vec3 v0(vec3 p)
{
    return 15.0*vec3(sin(p.y*10.0+time-10.0), -sin(p.x*10.0+9.0*time+10.0), +cos(2.4*p.z+2.0*time));
}

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
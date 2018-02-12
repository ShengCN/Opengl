#version 460
layout(location = 0) uniform mat4 PVM;
uniform int pass; 
uniform float iTime;

layout(location = 0) in vec3 pos_attrib;
layout(location = 1) in vec2 tex_coord_attrib;
layout(location = 2) in vec3 normal_attrib;

out vec2 tex_coord; 
out vec3 normal; 
flat out int instance_id;

uniform float currentID;

const float PI = 3.1415926;

mat3 m = mat3( 0.00,  0.80,  0.60,
              -0.80,  0.36, -0.48,
              -0.60, -0.48,  0.64 );

float hash( float n )
{
    return fract(sin(n)*43758.5453);
}

float noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);

    f = f*f*(3.0-2.0*f);

    float n = p.x + p.y*57.0 + 113.0*p.z;

    float res = mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                        mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y),
                    mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                        mix( hash(n+170.0), hash(n+171.0),f.x),f.y),f.z);
    return res;
}

float fbm( vec3 p )
{
    float f;
    f  = 0.5000*noise( p ); p = m*p*2.02;
    f += 0.2500*noise( p ); p = m*p*2.03;
    f += 0.1250*noise( p ); p = m*p*2.01;
    f += 0.0625*noise( p );
    return f;
}


mat3 rotateY(float radian)
{
    mat3 res = mat3(
        cos(radian),0.0,-sin(radian),
        0.0,1.0,0.0,
        sin(radian),0.0,cos(radian)
    );
    return res;
}

void main()
{
    int id = gl_InstanceID;

    // simple swim
    vec3 dis = vec3(0.0);
    dis.z = 0.5 * hash(id + 1.0) * sin(pos_attrib.x * 10.0 - iTime * 3.0 + fbm(vec3(id*756.97)) * 10.0); 


    // random swimming
    vec3 disV = vec3( sin(iTime + 10.0 * id) ,0.0, cos(iTime + 10.0 * id));
    vec3 center = vec3(fbm(vec3(id)),fbm(vec3(3.0+id)),fbm(vec3(90.0 + id))) * 4.0 - 2.0;
    dis += center + disV;
    vec3 forward = normalize(cross(disV,vec3(0.0,1.0,0.0)));
    float degree = acos(dot(forward,vec3(1.0,0.0,0.0)));

   vec3 pos = rotateY(-degree) * pos_attrib;
    
    // DEBUG
    // vec3 dis = vec3(id * 0.1);

    gl_Position = PVM * vec4(pos+dis,1.0);
    tex_coord = tex_coord_attrib;
    normal = normal_attrib;
    instance_id = id;
}
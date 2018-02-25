#version 430
layout(location = 1) uniform float iTime;
layout(location = 2) uniform vec2 iResolution;
layout(location = 3) uniform vec2 iMouse;
layout(location = 4)uniform vec4 fcolor;

out vec4 fragColor;

const float PI = 3.14159265;
const float TWO_PI = 6.2831853;
float epison = 0.00001;
const vec3 sphereOri = vec3(0.0);
const float r = 0.1;

uniform float slider;

float hash(float n)
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

float fbm(vec3 p)
{
      float f = 0.0;
      f += 0.5000 * noise(p); p*= 2.02 * 10.0;
      f += 0.2500 * noise(p); p*= 2.03;
      f += 0.1250 * noise(p); p*= 2.01;
      f += 0.0625 * noise(p);
      return f/0.9375;
}

vec3 buoyancy(vec3 p)
{
    return vec3(0.0,1.0,0.0);
}

vec3 blend_vector(vec3 v1, vec3 v2, float alpha)
{
    return alpha * v1 + (1.0 - alpha) * v2;
}

float sdSphere(vec3 p)
{
    return length(p-vec3(0.5,0.5,0.0)) - r;
}

float sdCircle(vec2 p)
{
    return length(p-vec2(0.5,0.5)) - r;
}

vec2 circleGradient(vec2 p)
{
    float e = 1e-4;
    vec2 dx = vec2(e,0.0);
    vec2 dy = vec2(0.0,e);

    float dxdt = (sdCircle(p+dx) - sdCircle(p-dx))/(2.0*e);
    float dydt = (sdCircle(p+dy) - sdCircle(p-dy))/(2.0*e);

    return vec2(dxdt,dydt);    
}

vec3 sphereGradient(vec3 p)
{
    float e = 1e-4;
    vec3 dx = vec3(e,0.0,0.0);
    vec3 dy = vec3(0.0,e,0.0);
    vec3 dz = vec3(0.0,0.0,e);

    float dxdt = (sdSphere(p+dx) - sdSphere(p-dx))/(2.0*e);
    float dydt = (sdSphere(p+dy) - sdSphere(p-dy))/(2.0*e);
    float dzdt = (sdSphere(p+dz) - sdSphere(p-dz))/(2.0*e);

    return vec3(dxdt,dydt,dzdt);
}

float ramp(float r)
{
    if(r>=1.0)
        return 1.0;
    else if(r<=-1.0)
        return -1.0;
        
    else
    {
        return 15.0/8.0 * r - 10.0/8.0 * pow(r,3) + 3.0 / 8.0 * pow(r,5);
    }
}

void main()
{
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    float ratio = iResolution.x/iResolution.y;
    vec3 color = vec3(0.0);
    
    // nosie potential
    vec3 seedX = vec3(uv,hash(0.3));
    vec3 seedY = vec3(uv,hash(0.6));
    vec3 seedZ = vec3(uv,hash(0.9));
    // vec3 color = vec3(noise(seedX),noise(seedY),noise(seedZ));    
    vec3 noisePotential = vec3(fbm(seedX),fbm(seedY),fbm(seedZ));

    // modulate noise and intensity
    vec3 ni = blend_vector(vec3(0.6),noisePotential,1.0 - uv.y);

    // boundaries
    vec3 sphereO = vec3 (0.5,0.5,0.0);
    float dis = smoothstep(r,0.35,length(sphereO.xy-uv));
    vec2 sphereG = circleGradient(uv); // normal vector
    
    // ramp coefficient
    float coe = ramp(dis);


    color = vec3(sphereG,0.6);
    fragColor = vec4(color, 1.0);
}
#version 430
layout(location = 1) uniform float time;
layout(location = 2) uniform vec2 resolution;
layout(location = 3) uniform vec2 mouse;
layout(location = 4)uniform vec4 fcolor;

out vec4 Frag_Color;

const float PI = 3.14159265;
const float TWO_PI = 6.2831853;
float epison = 0.00001;

float iSphere(vec3 ro, vec3 rd)
{
    // sphere equation: 
    // position: ro + rd * t
    // p^2 = ro^2 + 2 dot(ro,rd)*t + t^2 - r^2
    float r = 1.0;
    float a = 1.0;
    float b = 2 * dot(ro,rd);
    float c = dot(ro,ro) - r*r;
    float delta = b*b - 4*a*c;
    if(delta<0.0)
    {
        return -1.0;
    }
    else
    {
        return (-b - sqrt(delta)) /2*a;
    }
}

float iPlane(vec3 ro, vec3 rd)
{
    vec3 n = vec3(0.0,1.0,0.0);
    return -(dot(ro,n)/dot(rd,n));
}

float render(vec3 ro, vec3 rd)
{
    // positino = ro + rd * t
    float t = iSphere(ro,rd);
    // float t1 = iPlane(ro,rd);

    // if(t>t1 && t1 > 0.0) // first intersect plane
    //     return 2.0;
    // else if(t<t1&&t>0.0) // first interset sphere
    //     return 0.5;
    // else
    //     return -1.0;

    if(t>0.0)
        return 2.0;
}

void main()
{
    vec2 uv = gl_FragCoord.xy/resolution.xy;
    vec2 mouse_uv = mouse.xy/resolution.xy;
    mouse_uv.y = 1.0 - mouse_uv.y;

    // ray origin
    // pixel position
    // ray direction
    vec3 ro = vec3(0.0,0.0,3.0);
    vec3 rd = normalize(vec3(2.0*uv-1.0,-1.0));

    vec3 color = vec3(0.1);
    float id = render(ro,rd);
    // id
    if(id >0.0 && id <1.0)
    {
        color= vec3(0.0,0.0,1.0);
    }
    else if(id > 1.5)
    {
        color = vec3(1.0,0.0,0.0);
    }

    Frag_Color = vec4(color,1.0);
}
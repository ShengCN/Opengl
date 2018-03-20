#version 430
layout(location = 1) uniform float iTime;
layout(location = 2) uniform vec2 iResolution;
layout(location = 3) uniform vec2 iMouse;
layout(location = 4)uniform vec4 fcolor;

out vec4 fragColor;

const float PI = 3.14159265;
const float TWO_PI = 6.2831853;
float epison = 0.00001;


const float R = 0.3;

struct PotentialSource
{
	float val;
	vec3 pos;
};

uniform vec3 sourcePos1;
uniform vec3 sourcePos2;
uniform vec3 sourcePos3;
PotentialSource source[] = { PotentialSource(0.5f,sourcePos1*0.05),
							 PotentialSource(0.5f,sourcePos2*0.05),
							 PotentialSource(0.5f,sourcePos3*0.05)};


float Blend_Func(float r)
{
	return 1.0 - 0.444444 * pow(r, 6) / pow(R, 6) 
			   + 1.888889 * pow(r, 4) / pow(R, 4) 
		       - 2.444444 * pow(r, 2) / pow(R, 2);
}

float PotentialIntersect(vec3 p)
{
    float potential = Blend_Func(length((p-source[0].pos))) * source[0].val;
	potential += Blend_Func(length((p-source[1].pos))) * source[1].val;
	potential += Blend_Func(length((p-source[2].pos))) * source[2].val;

    return potential;
}

float iSphere(in vec3 ro, in vec3 rd, in vec4 sph)
{
    // sphere center at the origin
    // xyz = ro + t*rd;
    // vec3 oc = ro - sph.xyz;
    // float b = 2.0 * dot(oc,rd);
    // float c = dot(oc,oc) - sph.w*sph.w;
    // float h = b*b - 4.0*c;
    // if (h < 0.0) return -1.0;
    // float t = (-b - sqrt(h)) / 2.0;
    // return t;

    // Binary search for the intersect points
    float t = 0.0;
    float tl = 0.0;
    float tr = 10.0;
    while(true)
    {
        t = (tl + tr) * 0.5;
        float potential = PotentialIntersect(ro + t* rd);
        if(abs(potential-0.4)<epison)
        {
            break;
        }

        if(potential-0.4<0.0)
        {
            tl = t;
        }
        else
        {
            tr = t;
        }
    }

    return t;
}

vec3 nSphere(in vec3 pos, in vec4 sph)
{
    return (pos - sph.xyz) / sph.w;
}

float iPlane(in vec3 ro, in vec3 rd)
{
    // y = ro.y + t*rd.y = 0
    return -ro.y/rd.y;
}

vec3 nPlane(in vec3 pos)
{
    return vec3(0.0,1.0,0.0);
}

vec4 sph1 = vec4(0.0,1.0,0.0,1.0);
float  intersect(in vec3 ro, in vec3 rd, out float resT)
{	
    resT = 1000.0;
    float id = -1.0;
    float tsph = iSphere(ro,rd,sph1);
    float tpla = iPlane(ro,rd);
    if(tsph >0.0)
    {
        id = 1.0;
        resT = tsph;
    }
    if(tpla > 0.0 && tpla < resT)
    {
        id = 2.0;
        resT = tpla;
    }
    
    return id;
}


void main()
{
    vec3 light = normalize(vec3(0.57703));
    // sph1.x = 0.5 * sin(iTime);
    // sph1.z = 0.5 * cos(iTime);
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = gl_FragCoord.xy/iResolution.xy;
    // ray origin and ray direction
    vec3 ro = vec3(0.0,0.5,3.0);
    vec3 rd = normalize(vec3((-1.0 + 2.0 * uv)*vec2(iResolution.x/iResolution.y,1.0),-1.0));
    
    // intersect 
    float t;
    float id = intersect(ro,rd,t);
    
    vec3 col = vec3(0.0);
    vec3 pos = ro + t*rd;
    if(id > 0.5 && id <1.5)
    {
        // draw white when hit something
        vec3 nor = nSphere(pos,sph1);
        float dif = clamp(dot(nor,light),0.0,1.0);
        float ao = 0.5 + 0.5*nor.y;
        col = vec3(0.9,0.8,0.6)*dif*ao + vec3(0.1,0.2,0.4)*ao;
    }
    else if(id > 1.5)
    {
        vec3 nor = nPlane(pos);
        float dif = clamp(dot(nor,light),0.0,1.0);
        float amb = smoothstep(0.0,2.0*sph1.w,length(pos.xz - sph1.xz));
		col = vec3(amb*0.7);
    }
    col = sqrt(col);
    // Output to screen
    fragColor = vec4(col,0.5);
}
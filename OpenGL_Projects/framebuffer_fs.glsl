#version 430
layout(location = 1) uniform float time;
layout(location = 2) uniform vec2 resolution;
layout(location = 3) uniform vec2 mouse;
layout(location = 4)uniform vec4 fcolor;
uniform int pass;
uniform sampler2D color_tex;

in vec2 tex_coord;
in vec3 normal; 
out vec4 fragColor;

vec2 pos   = vec2(.94, .6);
vec2 scale = 1.5*vec2(.25,.375);
// --- noise functions from https://www.shadertoy.com/view/XslGRr
// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

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

float random (in vec2 _st) {
    return fract(sin(dot(_st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

#define NUM_OCTAVES 5

float noise2D (in vec2 _st) {
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

float fbm2D ( in vec2 _st) {
    float v = 0.0;
    float a = 0.5;
    vec2 shift = vec2(100.0);
    // Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.5), sin(0.5),
                    -sin(0.5), cos(0.50));
    for (int i = 0; i < NUM_OCTAVES; ++i) {
        v += a * noise2D(_st);
        _st = rot * _st * 2.0 + shift;
        a *= 0.5;
    }
    return v;
}
// --- End of Created by inigo quilez

// Source: https://www.shadertoy.com/view/4ds3D7
// Made by FabriceNeyret2 
vec2 fbm2( vec2 p )
{
	p += time;
    float fx = fbm(vec3(p,.5));
    float fy = fbm(vec3(p,.5)+vec3(1345.67,0,45.67));
    return vec2(fx,fy);
}

vec2 perturb2(vec2 p, float scaleX, float scaleI)
{
    scaleX *= 2.0;
	return scaleI*scaleX*fbm2(p/scaleX); 
}

bool isBorder(in ivec2 st)
{
	return st.x > 0.1 && st.x < 0.9 && st.y > 0.1 && st.y <0.9;
}

void main()
{
    vec2 uv = gl_FragCoord.xy/resolution.xy;
    vec2 mouse_uv = mouse.xy/resolution.xy;
    vec3 bg = vec3(250.0,243.0,225.0)/vec3(255.0);

    if(pass == 1)
	{
		vec3 l = vec3(0.707, 0.0, 0.707);
		float a = dot(normalize(normal), l) + 0.2;

		vec2 grid2 = smoothstep(0.0, 0.05, vec2(0.5)-abs(fract(10.0*tex_coord)-vec2(0.5)));
		float grid = 1.0-min(grid2.x, grid2.y) + 0.2;

		fragColor = vec4(vec3(grid*a), 1.0);
	}
    else if(pass == 2)
    {
        vec2 st = uv;
		pos *= resolution.xy;
		scale *= resolution.xy;
		st += perturb2((uv+pos/scale)-vec2(0.,time*0.1),.1,1.5);  

		ivec2 pixel_uv = ivec2(st*gl_FragCoord.xy) + ivec2(10.0*sin(5.0*time + 0.1*st.y*gl_FragCoord.y),0.0);
		vec4 water_effect = texelFetch(color_tex,pixel_uv,0);
		
		// edge detect
		vec4 col = vec4(0.0);
		if(pixel_uv.x > 2.0 && pixel_uv.x<1278.0 && pixel_uv.y>1.0 && pixel_uv.y < 718.0)
		{
			vec4 left = texelFetch(color_tex,pixel_uv + ivec2(-1.0,0.0),0);
			vec4 right = texelFetch(color_tex,pixel_uv + ivec2(1.0,0.0),0);
			vec4 above = texelFetch(color_tex,pixel_uv + ivec2(0.0,1.0),0);
			vec4 below = texelFetch(color_tex,pixel_uv + ivec2(0.0,-1.0),0);
			col = (left-right)*(left-right) + (above-below)*(above-below);
		}

		vec4 colT = sqrt(sqrt(col));
		if(length(colT.xyz)>0.5)
		{
			// fragColor = vec4(uv,0.0,1.0) * sin(0.1*time);
			//vec3 uvripColor = 0.5 + 0.5  * cos(time*0.001 + uv.xyx + vec3(0,2,4));
			// vec3 linCol = 2.0*vec3(5.0,0.6,0.0);
			vec3 uvripColor = vec3(0.7,0.5,0.0) * pow(pow(sin(time*0.5),2.0),2.0) * 30.0;
			fragColor = vec4(uvripColor,1.0);
		}
		else
		 	fragColor = col;
		
		// fragColor += water_effect;
		fragColor  = mix(fragColor,water_effect,0.3);

		// clouds
		vec3 cloudColor = vec3(0.0);
		vec2 q = vec2(0.);
   	 	q.x = fbm2D( uv + 0.00*time);
    	q.y = fbm2D( uv + vec2(1.0));

    	vec2 r = vec2(0.);
    	r.x = fbm2D( uv + 1.0*q + vec2(1.7,9.2)+ 0.15*time );
    	r.y = fbm2D( uv + 1.0*q + vec2(8.3,2.8)+ 0.126*time);

    	float f = fbm2D(uv+r);

    	cloudColor = mix(vec3(0.101961,0.619608,0.666667),
                vec3(0.666667,0.666667,0.498039),
                clamp((f*f)*4.0,0.0,1.0));

    	cloudColor = mix(cloudColor,
                vec3(0,0,0.164706),
                clamp(length(q),0.0,1.0));

    	cloudColor = mix(cloudColor,
                vec3(0.666667,1,1),
                clamp(length(r.x),0.0,1.0));

		cloudColor = (f*f*f+.6*f*f+.5*f)*cloudColor;
		// cloudColor = pow( clamp(cloudColor,0.0,1.0), vec3(0.45) );
		
        fragColor = mix(fragColor,vec4(cloudColor,1.0),0.95);
    }
}
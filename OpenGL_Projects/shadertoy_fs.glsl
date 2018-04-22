#version 430
layout(location = 1) uniform float iTime;
layout(location = 2) uniform vec2 iResolution;
layout(location = 3) uniform vec2 iMouse;
layout(location = 4)uniform vec4 fcolor;

uniform sampler2D volume_tex;
uniform sampler2D noise_tex;
uniform int slice;
uniform vec4 camera;
uniform vec4 slider;
uniform float angle;
uniform int octave;
uniform vec3 camera_pos;

out vec4 fragColor;

const vec3 LightPos = vec3(7.0,6.5,1.4) *0.1; 
const vec3 WaterColor = vec3(0.4, 0.9, 1);
const vec3 kSunDir = vec3(-0.624695,0.468521,-0.624695);
const float waterSZ = 1.0;
const float waterHeight = 0.8;
const float MaxWaveAmplitude = 0.04;
const float SC = 25.0;
const float kMaxTreeHeight = 2.0;
//#define LOWQUALITY

// ***********************
// Data Structures
// ***********************
struct MaterialInfo
{
	vec3 Kd;
	float Shininess;
};

// ***********************
// Functions
// ***********************
vec2 Intersect(vec3 ro, vec3 rd,float tmin, float tmax);
vec2 map(vec3 pos);

// ***********************
//	Tool functions
// ***********************
float sdEllipsoidY( in vec3 p, in vec2 r )
{
    return (length( p/r.xyx ) - 1.0) * r.x;
}

// return smoothstep and its derivative
vec2 smoothstepd( float a, float b, float x)
{
	if( x<a ) return vec2( 0.0, 0.0 );
	if( x>b ) return vec2( 1.0, 0.0 );
    float ir = 1.0/(b-a);
    x = (x-a)*ir;
    return vec2( x*x*(3.0-2.0*x), 6.0*x*(1.0-x)*ir );
}

float CyclicTime()
{
	return abs(mod(iTime, 30.)-15.0);
}

vec3 calcNormal(vec3 p)
{
    vec3 e = vec3(0.001,0.0,0.0);
    vec3 n;
    n.x = map(p+e.xyy).x - map(p-e.xyy).x;
    n.y = map(p+e.yxy).x - map(p-e.yxy).x;
    n.z = map(p+e.yyx).x - map(p-e.yyx).x;
    return normalize(n);
}

vec3 fog(in vec3 col, float t)
{
	vec3 fogCol = vec3(0.4,0.6,1.15);
	return mix( col, fogCol, 1.0-exp(-0.000001*t*t) );
}

float hash1( vec2 p )
{
    p  = 50.0*fract( p*0.3183099 );
    return fract( p.x*p.y*(p.x+p.y) );
}

float hash1( float n )
{
    return fract( n*17.0*fract( n*0.3183099 ) );
}

vec2 hash2( float n ) { return fract(sin(vec2(n,n+1.0))*vec2(43758.5453123,22578.1459123)); }


vec2 hash2( vec2 p ) 
{
    const vec2 k = vec2( 0.3183099, 0.3678794 );
    p = p*k + k.yx;
    return fract( 16.0 * k*fract( p.x*p.y*(p.x+p.y)) );
}

//==========================================================================================
// noises
//==========================================================================================

// value noise, and its analytical derivatives
vec4 noised( in vec3 x )
{
    vec3 p = floor(x);
    vec3 w = fract(x);
    
    vec3 u = w*w*w*(w*(w*6.0-15.0)+10.0);
    vec3 du = 30.0*w*w*(w*(w-2.0)+1.0);

    float n = p.x + 317.0*p.y + 157.0*p.z;
    
    float a = hash1(n+0.0);
    float b = hash1(n+1.0);
    float c = hash1(n+317.0);
    float d = hash1(n+318.0);
    float e = hash1(n+157.0);
	float f = hash1(n+158.0);
    float g = hash1(n+474.0);
    float h = hash1(n+475.0);

    float k0 =   a;
    float k1 =   b - a;
    float k2 =   c - a;
    float k3 =   e - a;
    float k4 =   a - b - c + d;
    float k5 =   a - c - e + g;
    float k6 =   a - b - e + f;
    float k7 = - a + b + c - d + e - f - g + h;

    return vec4( -1.0+2.0*(k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z), 
                      2.0* du * vec3( k1 + k4*u.y + k6*u.z + k7*u.y*u.z,
                                      k2 + k5*u.z + k4*u.x + k7*u.z*u.x,
                                      k3 + k6*u.x + k5*u.y + k7*u.x*u.y ) );
}

float noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 w = fract(x);
    
    vec3 u = w*w*w*(w*(w*6.0-15.0)+10.0);
    
    float n = p.x + 317.0*p.y + 157.0*p.z;
    
    float a = hash1(n+0.0);
    float b = hash1(n+1.0);
    float c = hash1(n+317.0);
    float d = hash1(n+318.0);
    float e = hash1(n+157.0);
	float f = hash1(n+158.0);
    float g = hash1(n+474.0);
    float h = hash1(n+475.0);

    float k0 =   a;
    float k1 =   b - a;
    float k2 =   c - a;
    float k3 =   e - a;
    float k4 =   a - b - c + d;
    float k5 =   a - c - e + g;
    float k6 =   a - b - e + f;
    float k7 = - a + b + c - d + e - f - g + h;

    return -1.0+2.0*(k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z);
}

vec3 noised( in vec2 x )
{
    vec2 p = floor(x);
    vec2 w = fract(x);
    
    vec2 u = w*w*w*(w*(w*6.0-15.0)+10.0);
    vec2 du = 30.0*w*w*(w*(w-2.0)+1.0);
    
    float a = hash1(p+vec2(0,0));
    float b = hash1(p+vec2(1,0));
    float c = hash1(p+vec2(0,1));
    float d = hash1(p+vec2(1,1));

    float k0 = a;
    float k1 = b - a;
    float k2 = c - a;
    float k4 = a - b - c + d;

    return vec3( -1.0+2.0*(k0 + k1*u.x + k2*u.y + k4*u.x*u.y), 
                      2.0* du * vec2( k1 + k4*u.y,
                                      k2 + k4*u.x ) );
}

float noise( in vec2 x )
{
    vec2 p = floor(x);
    vec2 w = fract(x);
    vec2 u = w*w*w*(w*(w*6.0-15.0)+10.0);
    
#if 0
    p *= 0.3183099;
    float kx0 = 50.0*fract( p.x );
    float kx1 = 50.0*fract( p.x+0.3183099 );
    float ky0 = 50.0*fract( p.y );
    float ky1 = 50.0*fract( p.y+0.3183099 );

    float a = fract( kx0*ky0*(kx0+ky0) );
    float b = fract( kx1*ky0*(kx1+ky0) );
    float c = fract( kx0*ky1*(kx0+ky1) );
    float d = fract( kx1*ky1*(kx1+ky1) );
#else
    float a = hash1(p+vec2(0,0));
    float b = hash1(p+vec2(1,0));
    float c = hash1(p+vec2(0,1));
    float d = hash1(p+vec2(1,1));
#endif
    
    return -1.0+2.0*( a + (b-a)*u.x + (c-a)*u.y + (a - b - c + d)*u.x*u.y );
}

//==========================================================================================
// fbm constructions
//==========================================================================================

const mat3 m3  = mat3( 0.00,  0.80,  0.60,
                      -0.80,  0.36, -0.48,
                      -0.60, -0.48,  0.64 );
const mat3 m3i = mat3( 0.00, -0.80, -0.60,
                       0.80,  0.36, -0.48,
                       0.60, -0.48,  0.64 );
const mat2 m2 = mat2(  0.80,  0.60,
                      -0.60,  0.80 );
const mat2 m2i = mat2( 0.80, -0.60,
                       0.60,  0.80 );

//------------------------------------------------------------------------------------------

float fbm_4( in vec3 x )
{
    float f = 2.0;
    float s = 0.5;
    float a = 0.0;
    float b = 0.5;
    for( int i=0; i<4; i++ )
    {
        float n = noise(x);
        a += b*n;
        b *= s;
        x = f*m3*x;
    }
	return a;
}

vec4 fbmd_8( in vec3 x )
{
    float f = 1.92;
    float s = 0.5;
    float a = 0.0;
    float b = 0.5;
    vec3  d = vec3(0.0);
    mat3  m = mat3(1.0,0.0,0.0,
                   0.0,1.0,0.0,
                   0.0,0.0,1.0);
    for( int i=0; i<7; i++ )
    {
        vec4 n = noised(x);
        a += b*n.x;          // accumulate values		
        d += b*m*n.yzw;      // accumulate derivatives
        b *= s;
        x = f*m3*x;
        m = f*m3i*m;
    }
	return vec4( a, d );
}

float fbm_9( in vec2 x )
{
    float f = 1.9;
    float s = 0.55;
    float a = 0.0;
    float b = 0.5;
    for( int i=0; i<9; i++ )
    {
        float n = noise(x);
        a += b*n;
        b *= s;
        x = f*m2*x;
    }
	return a;
}

vec3 fbmd_9( in vec2 x )
{
    float f = 1.9;
    float s = 0.55;
    float a = 0.0;
    float b = 0.5;
    vec2  d = vec2(0.0);
    mat2  m = mat2(1.0,0.0,0.0,1.0);
    for( int i=0; i<9; i++ )
    {
        vec3 n = noised(x);
        a += b*n.x;          // accumulate values		
        d += b*m*n.yz;       // accumulate derivatives
        b *= s;
        x = f*m2*x;
        m = f*m2i*m;
    }
	return vec3( a, d );
}

float fbm_4( in vec2 x )
{
    float f = 1.9;
    float s = 0.55;
    float a = 0.0;
    float b = 0.5;
    for( int i=0; i<4; i++ )
    {
        float n = noise(x);
        a += b*n;
        b *= s;
        x = f*m2*x;
    }
	return a;
}

// ***********************
//	SD functions
// ***********************
float WaveAmplitude() 
{
	return MaxWaveAmplitude * exp(-CyclicTime() / 10.);
}

float WaterWave(vec3 a) 
{
	return WaveAmplitude() * sin((2. * a.x * a.x + 2. * a.z * a.z) - 10. * CyclicTime());
}

float sdWaterSurface(vec3 pos)
{
	pos.y -= 0.8;
	vec3 sz = vec3(waterSZ,0.0,waterSZ);
	return length(max(abs(pos+vec3(0.0,WaterWave(pos),0.0)) - sz, 0.));
}

float sdSphere(vec3 pos)
{
	return length(pos-vec3(0.0,0.0,0.0)) - 0.5;
}

float sdPlane(vec3 pos)
{
	vec4 plane = vec4(0.0,1.0,0.0,0.0);
	//vec3 nor = noise(pos.xz);
	return dot(pos,plane.xyz) + 0.5;
}

bool IsWater(vec3 pos)
{
	return (pos.y < (waterHeight-MaxWaveAmplitude));
}

bool FloatEqual(float a, float b)
{
	return abs(a-b)<0.1;
}

/**********************
	Lighting and colors
**********************/
vec2 softShadow(in vec3 ro, in vec3 rd)
{
	vec2 res = vec2(1.0,0.0);
	for(float t = 0.01; t < 150.0;)
	{
		vec2 h = map(ro+t*rd);
		if(h.x<0.0001) return vec2(0.0,h.y);
        if(!FloatEqual(h.y,3.0)) res.x = min(res.x,6.0 * h.x / t);
        t += h.x;
	}
	return res;
}

vec2 refractIntersect(in vec3 ro, in vec3 rd)
{
	for(float t = 0.01; t < 15.0;)
	{
		vec2 h = map(ro+t*rd);
		if(h.x < 0.0001) return vec2(t,h.y);
		t += h.x;
	}	
	return vec2(0.0);
}

vec3 Lighting(vec3 pos, vec3 normal, vec3 eye, MaterialInfo m, vec3 lightColor, vec3 lightPos)
{
	vec3 lightDir = lightPos - pos;
	vec3 nlightDir = normalize(lightDir);
	vec3 light = normalize(vec3(1.0,0.8,0.6));
	// light = nlightDir;
	vec3 viewDir = normalize(pos-eye);
	vec3 ref = reflect(viewDir,normal);

	// Phong light
	float amb = 0.6 + 0.5 * normal.y;
	float dif = max(0.0,dot(normal,light));
	float spe = pow(max(dot(light,ref),0.0),m.Shininess) * (m.Shininess + 8.) / 25.0;
	vec2 sha = softShadow(pos,light);
	if(FloatEqual(sha.y,3.0)) sha.x = 1.0;

	vec3 color = amb * vec3(0.3);
	color += dif * sha.x * m.Kd * lightColor;
	if(spe>0.0) color += spe * lightColor * 0.08;

	// return color / dot(lightDir,lightDir);
	return color;
}

/**********************
	0	background
	1	ground
	2   sphere     ---- for debug
**********************/
MaterialInfo Material(vec3 pos) {
	MaterialInfo m;

	vec2 itsct = map(pos);

	if(FloatEqual(itsct.y,1.0)) 			// plane
	{
		m.Kd = vec3(1.0);
		m.Shininess = 0.0;
	}

	if(FloatEqual(itsct.y,2.0))				// sphere
	{
		m.Kd = vec3(0.1);
		m.Shininess = 10.0;
	}

	if(FloatEqual(itsct.y,3.0))				// water
	{
		m.Kd = WaterColor;
		m.Shininess = 120.0;
	}

	return m;
}

vec2 terrainMap(in vec2 p)
{
	const float sca = 0.0010;		// scale
	const float amp = 300.0;		// amplitude

	p *= sca;
	float e = fbm_9(p+vec2(1.0,-2.0));						// noise factor in xz
	float a = 1.0 - smoothstep(0.12,0.13,abs(e+0.12)); 		// [0.87,0.88]
	e = e + 0.15*smoothstep(-0.08,-0.01,e);					// offset
	e *= amp;												
	return vec2(e,a);
}

vec4 terrainMapD(in vec2 p)
{
	const float sca = 0.0010;
    const float amp = 300.0;
    p *= sca;
    vec3 e = fbmd_9( p + vec2(1.0,-2.0) );
    vec2 c = smoothstepd( -0.08, -0.01, e.x );
	e.x = e.x + 0.15*c.x;
	e.yz = e.yz + 0.15*c.y*e.yz;    
    e.x *= amp;
    e.yz *= amp*sca;
    return vec4( e.x, normalize( vec3(-e.y,1.0,-e.z) ) );
}

vec3 terrainNormal(in vec2 pos)
{
#if 1
    return terrainMapD(pos).yzw;
#else    
    vec2 e = vec2(0.03,0.0);
	return normalize( vec3(terrainMap(pos-e.xy).x - terrainMap(pos+e.xy).x,
                           2.0*e.x,
                           terrainMap(pos-e.yx).x - terrainMap(pos+e.yx).x ) );
#endif  
}

// return terrain (distance,tree) 
vec2 raymarchTerrain(in vec3 ro, in vec3 rd, float tmin, float tmax)
{
    float dis, th;
    float t2 = -1.0;
    float t = tmin; 
    float ot = t;
    float odis = 0.0;
    float odis2 = 0.0;

	for(int i = 0; i < 400; i++)
	{
		th = 0.001*t;
		vec3 pos = ro + t*rd;
		vec2 env = terrainMap(pos.xz); 		// important
		float hei = env.x;

		//todo
		// tree envelope
		float dis2 = pos.y - (hei+kMaxTreeHeight*1.1);
		if(dis2<th)
		{
			if(t2<0.0)
			{
				t2 = ot + (th-odis2)*(t-ot)/(dis2-odis2);	// linear interpolation
			}
		}
		odis2 = dis2;

		// terrain
		dis = pos.y - hei;
		if(dis<th) break;

		ot = t;
		odis = dis;
		t += dis*0.8*(1.0-0.75*env.y);				// estimate distance
		if(t>tmax) break;
	}

	if(t>tmax) t = -1.0;
	else t = ot + (th-odis)*(t-ot)/(dis-odis);		// linear interpolation
	return vec2(t,t2);
}

float terrainShadow(in vec3 ro, in vec3 rd, in float mint)
{
	float res = 1.0;
	float t = mint;
#ifdef	LOWQUALITY
	for(int i = 0; i < 32; i++)
	{
		vec3 pos = ro + t*rd;
		vec2 env = terrainMap(pos.xz);
		float hei = pos.y - env.x;
		res = min(res,32.0*hei/t);
		if(res<0.001) break;
		t += clamp(hei,1.0+t*0.1,50.0);
	}
#else
		for(int i = 0; i < 32; i++)
	{
		vec3 pos = ro + t*rd;
		vec2 env = terrainMap(pos.xz);
		float hei = pos.y - env.x;
		res = min(res,32.0*hei/t);
		if(res<0.0001) break;
		t += clamp( hei, 0.5+t*0.05, 25.0 );
	}
#endif
    return clamp( res, 0.0, 1.0 );
}

vec4 renderTerrain(in vec3 ro, in vec3 rd, in vec2 tmima, out float teShadow,out vec2 teDistance,inout float resT)
{
	vec4 res = vec4(0.0);
	teShadow = 0.0;
	teDistance = vec2(0.0);

	vec2 t = raymarchTerrain(ro,rd,tmima.x,tmima.y);
	if(t.x>0.0)	// intersect with terrain
	{
		vec3 pos = ro + t.x * rd;
		vec3 nor = terrainNormal(pos.xz);

		// bump map
		nor = normalize( nor + 0.8*(1.0-abs(nor.y))*0.8*fbmd_8( pos*0.3*vec3(1.0,0.2,1.0) ).yzw );
		vec3 col = vec3(0.18,0.11,0.10)*0.75;
		//vec3 col = vec3(79.0, 65.0, 57.0)/255.0;
		col = 1.0*mix(col,vec3(0.1,0.1,0.0)*0.3,smoothstep(0.7,0.9,nor.y));

		// shadow
		float sha = 0.0;
		float dif = clamp(dot(nor,kSunDir),0.0,1.0);
		if(dif > 0.001)
		{
			sha = terrainShadow(pos+nor*0.01,kSunDir,0.001);
			dif *= sha;
		}
		vec3  ref = reflect(rd,nor);
    	float bac = clamp( dot(normalize(vec3(-kSunDir.x,0.0,-kSunDir.z)),nor), 0.0, 1.0 )*clamp( (pos.y+100.0)/100.0, 0.0,1.0);
        float dom = clamp( 0.5 + 0.5*nor.y, 0.0, 1.0 );
        vec3  lin  = 1.0*0.2*mix(0.1*vec3(0.1,0.2,0.0),vec3(0.7,0.9,1.0),dom);//pow(vec3(occ),vec3(1.5,0.7,0.5));
              lin += 1.0*5.0*vec3(1.0,0.9,0.8)*dif;        
              lin += 1.0*0.35*vec3(1.0)*bac;
        
	    col *= lin;

        col = fog(col,t.x);

        teShadow = sha;
        teDistance = t;
        res = vec4( col, 1.0 );
        resT = t.x;
	}

	return res;
}

float treesMap( in vec3 p, in float rt, out float oHei, out float oMat, out float oDis )
{
	oHei = 1.0;
    oDis = 0.1;
    oMat = 0.0;

	float base = terrainMap(p.xz).x; 
    
    float d = 10.0;
    vec2 n = floor( p.xz );
    vec2 f = fract( p.xz );
    for( int j=-1; j<=1; j++ )
    for( int i=-1; i<=1; i++ )
    {
        vec2  g = vec2( float(i), float(j) );
        vec2  o = hash2( n + g );
        vec2  v = hash2( n +g + vec2(13.1,71.7) );
        vec2  r = g - f + o;

        float height = kMaxTreeHeight * (0.4+0.8*v.x) * (1.0 +cos(iTime)*0.1);
        float width = 0.9*(0.5 + 0.2*v.x + 0.3*v.y);
        vec3  q = vec3(r.x,p.y-base-height*0.5,r.y);
        float k = sdEllipsoidY( q, vec2(width,0.5*height) );

        if( k<d )
        { 
            d = k;
            //oMat = hash1(o); //fract(o.x*7.0 + o.y*15.0);
            oMat = o.x*7.0 + o.y*15.0;
            oHei = (p.y - base)/height;
            oHei *= 0.5 + 0.5*length(q) / width;
        }
    }
    oMat = fract(oMat);

    // distort ellipsoids to make them look like trees (works only in the distance really)
    #ifdef LOWQUALITY
    if( rt<350.0 )
    #else
    if( rt<500.0 )
    #endif
    {
        float s = fbm_4( p*3.0 );
        s = s*s;
        oDis = s;
        #ifdef LOWQUALITY
        float att = 1.0-smoothstep(150.0,350.0,rt);
        #else
        float att = 1.0-smoothstep(200.0,500.0,rt);
        #endif
        d += 2.0*s*att*att;
    }
    
    return d;
}

float treesShadow( in vec3 ro, in vec3 rd )
{
    float res = 1.0;
    float t = 0.02;
#ifdef LOWQUALITY
    for( int i=0; i<50; i++ )
    {
        float kk1, kk2, kk3;
        float h = treesMap( ro + rd*t, t, kk1, kk2, kk3 );
        res = min( res, 32.0*h/t );
        t += h;
        if( res<0.001 || t>20.0 ) break;
    }
#else
    for( int i=0; i<150; i++ )
    {
        float kk1, kk2, kk3;
        float h = treesMap( ro + rd*t, t, kk1, kk2, kk3 );
        res = min( res, 32.0*h/t );
        t += h;
        if( res<0.001 || t>120.0 ) break;
    }
#endif
    return clamp( res, 0.0, 1.0 );
}


vec3 treesNormal(in vec3 pos,in float t)
{
	const float eps = 0.005;
	vec2 e = vec2(1.0,-1.0)*0.5773*eps;
	float kk1, kk2, kk3;
	return normalize( e.xyy*treesMap( pos + e.xyy, t, kk1, kk2, kk3 ) + 
                      e.yyx*treesMap( pos + e.yyx, t, kk1, kk2, kk3 ) + 
                      e.yxy*treesMap( pos + e.yxy, t, kk1, kk2, kk3 ) + 
                      e.xxx*treesMap( pos + e.xxx, t, kk1, kk2, kk3 ) );    
}

vec3 treesShade( in vec3 pos, in vec3 tnor, in vec3 enor, in float hei, in float mid, in float dis, in float rt, in vec3 rd, float terrainShadow )
{
	vec3 nor = normalize( tnor + 2.5*enor );

    // --- lighting ---
    float sha = terrainShadow;
    vec3  ref = reflect(rd,nor);
    float occ = clamp(hei,0.0,1.0) * pow(1.0-2.0*dis,3.0);
    float dif = clamp( 0.1 + 0.9*dot( nor, kSunDir), 0.0, 1.0 ); 
    if( dif>0.0001 && terrainShadow>0.001 )
    {
        //sha *= clamp( 10.0*dot(tnor,kSunDir), 0.0, 1.0 ) * pow(clamp(1.0-13.0*dis,0.0,1.0),4.0);//treesShadow( pos+nor*0.1, kSunDir ); // only cast in non-terrain-occluded areas
        sha *= treesShadow( pos+nor*0.1, kSunDir ); // only cast in non-terrain-occluded areas
    }
    float dom = clamp( 0.5 + 0.5*nor.y, 0.0, 1.0 );
    float fre = clamp(1.0+dot(nor,rd),0.0,1.0);
    float spe = pow( clamp(dot(ref,kSunDir),0.0, 1.0), 9.0 )*dif*sha*(0.2+0.8*pow(fre,5.0))*occ;

    // --- lights ---
    vec3 lin  = 1.0*0.5*mix(0.1*vec3(0.1,0.2,0.0),vec3(0.6,1.0,1.0),dom*occ);
		 #ifdef SOFTTREES
         lin += 1.0*15.0*vec3(1.0,0.9,0.8)*dif*occ*sha;
		 #else
         lin += 1.0*10.0*vec3(1.0,0.9,0.8)*dif*occ*sha;
		 #endif
         lin += 1.0*0.5*vec3(0.9,1.0,0.8)*pow(fre,3.0)*occ;
         lin += 1.0*0.05*vec3(0.15,0.4,0.1)*occ;
   
    // --- material ---
    float brownAreas = fbm_4( pos.zx*0.03 );
	vec3 green = vec3(0.08,0.09,0.02);
	vec3 brown = vec3(243.0, 18.0, 27.0)/255.0*0.1;

	float test = abs(sin(iTime*0.1))-0.3;
    vec3 col = (1.0-test)*green + test*brown;
         col *= 1.6;

    // --- brdf * material ---
       col *= lin;
       col += spe*1.2*vec3(1.0,1.1,2.5);

    // // --- fog ---
    col = fog( col, rt );

    return col;
}

vec4 renderTrees(in vec3 ro, in vec3 rd, float tmin, float tmax, float terrainShadow,inout float resT)
{
	// find intersection
	float t = tmin;
	float hei, mid, displa;

	for(int i = 0; i < 64; i++)
	{
		vec3 pos = ro + t*rd;
		float dis = treesMap(pos,t,hei,mid,displa);
		if(dis<0.0001*t) break;
		t += dis;
		
		// not found
		if(t>tmax)	return vec4(0.0);
	}

	// shading
	vec3 pos = ro + t*rd;
	vec3 enor = terrainNormal(pos.xz);
	vec3 tnor = treesNormal(pos,t);
	vec3 col = treesShade(pos,tnor,enor,hei,mid,displa,t,rd,terrainShadow);
	resT = t;

	return vec4(col,1.0);
}

vec3 renderSky(in vec3 ro, in vec3 rd)
{
	// background sky
	vec3 col = 0.9*vec3(0.4,0.65,1.0) - rd.y*vec3(0.4,0.36,0.4);

	// clouds
	float t = (1000.0-ro.y)/ro.y;
	if(t>0.0)
	{
		vec2 uv = (ro+t*rd).xz;
		float cl = fbm_9(uv*0.002);
		float dl = smoothstep(-0.2,0.6,cl);
		col = mix(col,vec3(1.0),0.4*dl);
	}

	// sun glare
	float sun = clamp(dot(kSunDir,rd),0.0,1.0);
	col += 0.6*vec3(1.0,0.6,0.3)*pow(sun,32.0);

	return col;
}

//-------------------------------
// Clouds
//-------------------------------
// clouds density and its gradient
vec4 cloudsMap(in vec3 pos)
{
	vec3 p = pos + vec3(CyclicTime(),CyclicTime(),CyclicTime())*5.0;
	vec4 n = fbmd_8(p*0.003*vec3(0.6,1.0,0.6)-vec3(0.1,1.9,2.8));
	vec2 h = smoothstepd(-60,10.0,p.y) - smoothstepd(10.0,500.0,p.y);
    h.x = 2.0*n.x + h.x - 1.4;
    return vec4( h.x, 2.0*n.yzw*vec3(0.6,1.0,0.6)*0.003 + vec3(0.0,h.y,0.0)  );
}

float cloudsShadow(in vec3 ro, in vec3 rd, float tmin, float tmax)
{
	float sum = 0.0;
	float tl = (-10.0-ro.y)/rd.y; 				// min height steps
	float th = (300.0-ro.y)/rd.y;
	if(tl>0.0) tmin = max(tmin,tl);
	tmax = min(tmax,th);	

	float t = tmin;
	for(int i = 0; i<64; i++)
	{
		vec3 pos = ro + t*rd;
		vec4 denGra = cloudsMap(pos);
		float den = denGra.x;
		float dt = max(0.2,0.02*t);
		if(den>0.001)
		{
			float alp = clamp(den*0.3*min(dt,tmax-t-dt),0.0,1.0);
			sum = sum + alp*(1.0-sum);
		}
		else
		{
			dt *= 1.0 + 4.0*abs(den);
		}
		t += dt;
		if(sum>0.995 || t>tmax) break;
	}
	return clamp(1.0-sum,0.0,1.0);
}

// volume rendering clouds
vec4 renderClouds(in vec3 ro, in vec3 rd, float tmin, float tmax, inout float resT)
{
	vec4 sum = vec4(0.0);

	// bouding volume min height and max height
	float tl = (-30.0-ro.y)/rd.y; 				// min height steps
	float th = (300.0-ro.y)/rd.y;
	if(tl>0.0) tmin = max(tmin,tl);	else return sum;
	tmax = min(tmax,th);

	float t = tmin;
	float lastT =t;
	float thickness = 0.0;
	#ifdef LOWQUALITY
	for(int i = 0; i < 128; i++)
	#else
	for(int i = 0; i < 300; i++)
	#endif
	{
		vec3 pos = ro + t*rd;
		vec4 denGra = cloudsMap(pos); 		// density gradient
		float den = denGra.x;
		#ifdef LOWQUALITY
		float dt = max(0.1,0.011*t);
		#else
		float dt = max(0.05,0.005*t);
		#endif
		if(den>0.001)
		{
			#ifdef LOWQUALITY				// no shadow
			float sha = 1.0;
			#else
			float sha = clamp(1.0 - max(0.0,cloudsMap(pos+kSunDir*5.0).x),0.0,1.0);
			#endif
			vec3 nor = -normalize(denGra.yzw);
			float dif = clamp(dot(nor,kSunDir),0.0,1.0)*sha;
			float fre = clamp(1.0+dot(nor,rd),0.0,1.0)*sha;
			// lighting
			vec3 lin = vec3(0.70,0.80,1.00)*0.9*(0.6+0.4*nor.y);
			lin += vec3(0.20,0.25,0.20)*0.7*(0.5-0.5*nor.y);
			lin += vec3(1.00,0.70,0.40)*4.5*dif*(1.0-den);
			lin += vec3(0.80,0.70,0.50)*1.3*pow(fre,32.0)*(1.0-den);
			// color
			vec3 col = vec3(0.8,0.77,0.72)*clamp(1.0-4.0*den,0.0,1.0);
			col *= lin;
			col = fog(col,t);

			// volume integral
			float alp = clamp(den*0.25*min(dt,tmax-t-dt),0.0,1.0);
			col.rgb *= alp;
			sum = sum + vec4(col,alp)*(1.0-sum.a);

			thickness += dt*den;
			lastT = t;
		}
		else
		{
			#ifdef LOWQUALITY
			dt *= 1.0 + 4.0*abs(den);
			#else
			dt *= 0.8 + 2.0*abs(den);
			#endif
		}
		t += dt;
		if(sum.a>0.995 || t>tmax)	break;
	}

	resT = mix(resT,lastT,sum.w);
	if( thickness>0.0)
		sum.xyz += vec3(1.00,0.60,0.40)*0.2*pow(clamp(dot(kSunDir,rd),0.0,1.0),32.0)*exp(-0.3*thickness)*clamp(thickness*4.0,0.0,1.0);

    return clamp( sum, 0.0, 1.0 );
}

vec3 Shade(vec3 ro, vec3 rd, vec2 t)
{
	float id = t.y;
	// normal calculate
	vec3 pos = ro + t.x * rd;
	vec3 nor = calcNormal(pos);
	MaterialInfo m = Material(pos);

	vec3 color 	= vec3(0.0);

	if(FloatEqual(id,0))					// sky
	{
		color = vec3(0.0);
	}
	if(FloatEqual(id,1.0))					// ground
	{
		color = Lighting(pos,nor,ro,m,vec3(1.0),LightPos);
	}
	if(FloatEqual(id,2.0))					// sphere
	{
		color = Lighting(pos,nor,ro,m,vec3(1.0),LightPos);
	}
	if(FloatEqual(id,3.0))					// water
	{
		vec3 surfaceColor = Lighting(pos,nor,ro,m,vec3(1.0),LightPos);
		bool isSurface = !IsWater(pos); 

		// refract
		vec3 refractionDir = refract(normalize(rd),nor,0.9);
		vec2 refractTrace = refractIntersect(pos,refractionDir); 
		pos += refractionDir * refractTrace.x;
		nor = calcNormal(pos);
		m = Material(pos);

		color = Lighting(pos,nor,ro,m,vec3(1.0),LightPos);
		color *= WaterColor;
		if(isSurface)	color+= surfaceColor;
	}

	return color;
}

vec2 map(vec3 pos)
{
	vec2 d1 = vec2(sdPlane(pos),1.0);
	vec2 d2 = vec2(sdSphere(pos),2.0);
	vec2 d3 = vec2(sdWaterSurface(pos),3.0);

	if(d2.x<d1.x) d1 = d2;
	if(d3.x<d1.x) d1 = d3;
	return d1;
}

vec2 Intersect(vec3 ro, vec3 rd,float tmin, float tmax)
{
	float t = tmin;
    for(int i = 0; i < 256; i++)
    {
		vec3 pos = ro + t * rd;
		vec2 h = map(pos);

        if(h.x<0.0001) return vec2(t,h.y);
        t += h.x;
		
		if(t>tmax)	break;
    }

    return vec2(0.0);
}

vec3 Camera()
{
	vec2 p = (-iResolution.xy + 2.0*gl_FragCoord.xy) / iResolution.y;

	// camera anim
	float cr = 3.0;
	float cTime = -angle * 3.1415 / 20.0;
    // float cTime = iTime * 0.5;
	vec3 ro = (vec3(cr * sin(cTime) , 2.0 + camera.y, cr * cos(cTime))+camera_pos)*(13.0+camera.w*5.0);
	// ro = vec3(0.0, -99.25, 5.0) + vec3(10.0*sin(0.02*iTime),0.0,-10.0*sin(0.2+0.031*iTime)) + vec3(cos(cTime),0.0,sin(cTime))*100.0;
	// vec3 ta = vec3(0.0, -98.25, -45.0 + ro.z );

	ro = vec3(0.0,-99.25,5.0) + camera_pos;
	vec3 ta = vec3(ro.x,ro.y + 1.0,-45.0 + ro.z);

    // camera matrix	
	vec3  cw = normalize( ta-ro );							 	// forward ray
	vec3  cu = normalize( cross(cw,vec3(0.0,1.0,0.0)) );		// x
	vec3  cv = normalize( cross(cu,cw) );						// y
	vec3  rd = normalize( p.x*cu + p.y*cv + 2.0 *cw);
	float resT = 1000.0; 										// ray max step

	// // sky
	vec3 col = renderSky(ro,rd); 
	
	// terrain
	vec2 teDistance;
	float teShadow;
	vec2 tmima = vec2(15.0,1000.0);
	{
		vec4 res = renderTerrain(ro,rd,tmima,teShadow,teDistance,resT);
		col = col * (1.0-res.w) + res.xyz;
	}

	// trees
	if(teDistance.y>0.0)
	{
		tmima = vec2(teDistance.y,(teDistance.x>0.0)?teDistance.x:tmima.y);
		vec4 res = renderTrees(ro,rd,tmima.x,tmima.y,teShadow,resT);
		col = col*(1.0 - res.w) + res.xyz;
	}

	//----------------------------------
    // clouds
    //----------------------------------
    {
        vec4 res = renderClouds( ro, rd, 0.0, (teDistance.x>0.0)?teDistance.x:tmima.y, resT );
        col = col*(1.0-res.w) + res.xyz;
    }

	// water, sphere
	// vec2 t = Intersect(ro,rd,0.0,1000.0);
	//col += Shade(ro,rd,t);

	// clouds

	return col;
}

void main()
{
	vec3 col = Camera();	
	//col = pow(col,vec3(1.0/2.2));
	// col += texture(noise_tex,gl_FragCoord.xy/256.0).xyz/100.0;
    fragColor=vec4(col,1.0);
}
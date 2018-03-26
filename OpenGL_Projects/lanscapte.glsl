#version 430
layout(location = 1) uniform float iTime;
layout(location = 2) uniform vec2 iResolution;
layout(location = 3) uniform vec2 iMouse;
layout(location = 4)uniform vec4 fcolor;

uniform float slider;
uniform vec4 spherePos;
uniform vec3 ro_offset;

out vec4 fragColor;

const float PI = 3.14159265;
const float TWO_PI = 6.2831853;
float epison = 0.00001;

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
      f += 0.5000 * noise(p); p*= 2.02;
      f += 0.2500 * noise(p); p*= 2.03;
      f += 0.1250 * noise(p); p*= 2.01;
      f += 0.0625 * noise(p);
      return f/0.9375;
}

vec3 fruitMaterial(vec3 pos, vec3 nor)
{
      float a = atan(pos.x,pos.z);
      float r = length(pos.xz);

      // red 
      vec3 col = vec3(1.0,0.0,0.0);

      // mix to green
      float f = smoothstep(0.2,1.0,fbm(pos));
      col = mix(col,vec3(0.8,1.0,0.2),f);

      // make it dirty
      f = smoothstep(0.1,1.0,fbm(pos*4.0));
      col *= 0.8 + 0.2 * f; 
    
      // frekles
      f = smoothstep(0.7,0.9, fbm(pos * 48.0));
      col = mix(col,vec3(0.9,0.9,0.6),f);

      return col;
}

vec3 floorMaterial(vec3 pos, vec3 nor)
{
      vec3 f = vec3(0.2);
      return f;
}

float sdSphere(in vec3 p)
{
      // vec3 so = 1.0*vec3(cos(iTime),0.0,sin(iTime));
      vec3 so = vec3(0.0);
      return length(p-so) - 1.0;
}

float sdFloor(in vec3 p)
{
    float floor_y = sin(p.x) * sin(p.z); 
    return p.y + floor_y;
}

// return vec2(distance, id)
vec2 map(in vec3 p)
{
      vec2 d1 = vec2(sdSphere(p),1.0);
      vec2 d2 = vec2(sdFloor(p),2.0);
      if(d2.x<d1.x) d1 = d2;

      return d1;
}

vec3 calcNormal(in vec3 p)
{
      vec3 e = vec3(0.001,0.0,0.0);
      vec3 n;
      n.x = map(p+e.xyy).x - map(p-e.xyy).x;
      n.y = map(p+e.yxy).x - map(p-e.yxy).x;
      n.z = map(p+e.yyx).x - map(p-e.yyx).x;
      return normalize(n);
}

float softShadow(in vec3 ro, in vec3 rd)
{
      float res = 1.0;
      for (float t = 0.1; t < 8.0;)
      {
            float h = map(ro + t*rd).x;
            if(h<0.001) return 0.0;
            res = min(res,6.0 * h/t);
            t += h;
      }
      return res;
}

// return object (t,id)
vec2 intersect(vec3 ro,vec3 rd)
{
      for(float t = 0.0; t < 100.0;)
      {
            vec2 h = map(ro+t*rd);
            if(h.x<0.0001) return vec2(t,h.y);
            t += h.x;
      }

      return vec2(0.0);
}
void main()
{
      vec2 uv = gl_FragCoord.xy / iResolution.xy;
      float ratio = iResolution.x/iResolution.y;
      vec2 p = (uv*2.0 - 1.0)*vec2(ratio,1.0);

      // camera
      vec3 ro = 8.0 * vec3(cos(iTime),0.5,sin(iTime));
      // vec3 ro = vec3(0.0,0.0,-4.0);
      vec3 ww = normalize(vec3(0.0)-ro);
      vec3 uu = normalize(cross(vec3(0.0,1.0,0.0),ww));
      vec3 vv = normalize(cross(ww,uu));
      vec3 rd = normalize(p.x * uu+p.y*vv+1.5*ww);

      vec3 color = vec3(0.0);
      vec2 t = intersect(ro,rd);

      if(t.y>0.5) // without sky
      {
            vec3 pos = ro + t.x * rd;
            vec3 nor = calcNormal(pos);
            vec3 light = vec3(1.0,0.8,0.6);
            vec3 ref = reflect(rd,nor);

            float amb = 0.6 + 0.5 * nor.y;
            float dif = max(0.0,dot(light,nor));            
            float spe = pow(clamp(dot(light,ref),0.0,1.0),64.0);

            float sha = softShadow(pos,light);

            color = amb * vec3(0.3);
            color += dif*vec3(1.0)*sha;
            
            color  = sqrt(color);
            color *= 0.6;

            color += 0.08 * pow(spe,1.0);
      }

	fragColor = vec4(color, 1.0);
}
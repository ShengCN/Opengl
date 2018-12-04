#version 430
layout(location = 1) uniform float iTime;
layout(location = 2) uniform vec2 iResolution;
layout(location = 3) uniform vec2 iMouse;
layout(location = 4)uniform vec4 fcolor;

uniform vec4 camera;
uniform vec4 slider;
uniform float angle;
uniform float globalTime;
uniform vec3 camera_pos;
uniform samplerCube cubemap;

out vec4 fragColor;

struct Ray{
	vec3 ro;
	vec3 rd;
};

void main()
{
	vec3 cameraC = vec3(0.0);
	float imgPlaneZ = 1.0;
	vec2 uv = 2.0 * gl_FragCoord.xy / iResolution - 1.0;
	Ray cameraRay;
	cameraRay.ro = cameraC;
	cameraRay.rd = vec3(uv,-1.0f);
	
	vec3 col = vec3(0.0);
	col = texture(cubemap, cameraRay.rd).rgb;
    // fragColor=vec4(col,1.0);
	fragColor=vec4(col,1.0);
}
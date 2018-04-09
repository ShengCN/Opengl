#version 430
layout(location = 1) uniform float iTime;
layout(location = 2) uniform vec2 iResolution;
layout(location = 3) uniform vec2 iMouse;
layout(location = 4)uniform vec4 fcolor;

uniform sampler2D volume_tex;
uniform int slice;

out vec4 fragColor;

void main()
{
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    uv = uv * 2.0 - 1.0;
   
	//fragColor = vec4(uv,1.0,1.0);
    //fragColor = texelFetch(volume_tex,ivec3(gl_FragCoord.xy,mod(iTime*2,200)),0);
    fragColor = texelFetch(volume_tex,ivec2(gl_FragCoord.xy),0);
}
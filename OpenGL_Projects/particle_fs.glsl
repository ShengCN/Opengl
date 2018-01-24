#version 400

uniform sampler2D texture;
uniform vec2 resolution;
uniform float time;
uniform vec2 mouse;

out vec4 fragcolor;           
in vec2 tex_coord;
in vec4 gl_FragCoord;

void main(void)
{   
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    vec2 mouseUV = mouse.xy / resolution.xy;
    mouseUV.y = 1.0 - mouseUV.y;

    //fragcolor = texture2D(texture, tex_coord);
    vec2 p = 2.0 * (fract(gl_PointCoord.xy) - vec2(0.5));
    if(length(p)>=0.9 || length(p)<=0.6)
        discard;

    float r = length(uv - mouseUV);
    if(r<0.1)
    {
        fragcolor = vec4(0.0,0.0,0.0,1.0);
       // gl_PointCoord.x += vec4(0.5,0.0,0.0,1.0);
    }
    else
        fragcolor = vec4(uv.x,uv.y,abs(sin(time)),1.0);
}





















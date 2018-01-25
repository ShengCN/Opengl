#version 430
layout(location = 1) uniform float time;
layout(location = 2) uniform vec2 resolution;
layout(location = 3) uniform vec2 mouse;
layout(location = 4)uniform vec4 fcolor;
layout(location = 5)uniform sampler2D texture;
        
in vec2 tex_coord;
in vec4 gl_FragCoord;
out vec4 fragcolor;   

float plot(vec2 st,float pct)
{
    return smoothstep(pct-0.02,pct,st.y) - smoothstep(pct,pct+0.02,st.y);
}

void Draw_Bubble()
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

void Draw_Plot()
{
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    vec2 mouseUV = mouse.xy / resolution.xy;
    mouseUV.y = 1.0 - mouseUV.y;

    //float y = pow(uv.x,2);
    float y = step(0.2,uv.x)- step(0.8,uv.x);
    vec3 color = vec3(y);
    float pct = plot(uv,y);
    color = (1.0-pct)*color + pct * vec3(0.0,1.0,0.0);
    fragcolor = vec4(color,1.0);
}

void Default()
{
    fragcolor = fcolor;
}

void main(void)
{   
    //Draw_Plot();
    //Draw_Bubble();
    Default();
}





















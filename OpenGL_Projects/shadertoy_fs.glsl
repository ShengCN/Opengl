#version 430
layout(location = 1) uniform float time;
layout(location = 2) uniform vec2 resolution;
layout(location = 3) uniform vec2 mouse;
layout(location = 4)uniform vec4 fcolor;

out vec4 Frag_Color;

const float PI = 3.14159265;
const float TWO_PI = 6.2831853;
vec3 red = vec3(1.0,0.0,0.0);
vec3 green = vec3(0.0,1.0,0.0);
vec3 blue = vec3(0.0,0.0,1.0);
vec3 colorA = vec3(0.149,0.141,0.912);
vec3 colorB = vec3(1.000,0.833,0.224);

float smoothplot(vec2 uv, float pct, float line_width)
{
    return smoothstep(pct-line_width,pct,uv.y) - smoothstep(pct,pct+line_width,uv.y);
}

float plot(float axis, float pct, float line_width)
{
    return step(pct-line_width,axis) - step(pct,axis);
}

vec3 mix3channel(vec3 color1, vec3 color2,vec3 p)
{
    vec3 ret;
    ret.x = mix(color1.x,color2.x,p.x);
    ret.y = mix(color1.y,color2.y,p.y);
    ret.z = mix(color1.z,color2.z,p.z);
    return ret;
}

vec3 hsb2rgb(in vec3 c)
{
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                             6.0)-3.0)-1.0,
                     0.0,
                     1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix( vec3(1.0), rgb, c.y);
}

// dark
vec3 draw_tri(vec2 uv, vec2 tP)
{
    vec2 lb = step(tP.x,uv);
    vec2 ru = step(1.0-tP.y,1-uv);
    return 1.0-vec3(lb.x*lb.y*ru.x*ru.y);
}

void draw_row(vec2 uv,inout vec3 color, vec3 linecolor, float width,float row)
{
    float pct = plot(uv.y,row,width);
    color = mix(color,linecolor,pct); 
}

void draw_col(vec2 uv,inout vec3 color, vec3 linecolor, float width,float col)
{
    float pct = plot(uv.x,col,width);
    color = mix(color,linecolor,pct); 
}

void draw_rec(vec2 uv, inout vec3 color,vec3 tColor, vec2 lb,vec2 ru)
{
    // Check if inside
    float row_inside = step(uv.y,lb.y) - step(uv.y,ru.y);
    float col_inside = step(uv.x,lb.x) - step(uv.x,ru.x);
    //color = mix(color,tColor,row_inside*col_inside);
    if(row_inside * col_inside > 0.1)
    {
        color = tColor;
    }
}

void main()
{
    vec2 uv = gl_FragCoord.xy/resolution.xy;
    vec2 mouse_uv = mouse.xy/resolution.xy;
    vec3 bg = vec3(250.0,243.0,225.0)/vec3(255.0);
    vec3 red = vec3(166.0,31.0,35.0)/vec3(255.0);
    vec3 blue = vec3(1.0,95.0,156.0)/vec3(255.0);
    vec3 yellow = vec3(249.0,194.0,41.0)/vec3(255.0);

    vec3 color = bg;

    float pct = plot(uv.y,0.8,0.02);
    vec3 black = vec3(0.0);
    float width = 0.02;
    
    draw_row(uv,color,black,width,0.8); // first row
    draw_row(uv,color,black,width,0.6); // second row
    draw_row(uv,color,black,width,0.1); // third row

    draw_col(uv,color,black,width,0.07); // first col
    draw_col(uv,color,black,width,0.2);  // second col
    draw_col(uv,color,black,width,0.8);  // thrid col
    draw_col(uv,color,black,width,0.98); // third col
    
    draw_rec(uv,color,bg,vec2(0.0),vec2(0.2-width,0.6-width));//left bottom
    draw_rec(uv,color,red,vec2(0.0,0.8),vec2(0.07-width,1.0));//left up
    draw_rec(uv,color,red,vec2(0.07,0.8),vec2(0.2-width,1.0));
    draw_rec(uv,color,red,vec2(0.0,0.6),vec2(0.07-width,0.8-width));
    draw_rec(uv,color,red,vec2(0.07,0.6),vec2(0.2-width,0.8-width));

    draw_rec(uv,color,yellow,vec2(0.98,0.8),vec2(1.0,1.0));
    draw_rec(uv,color,yellow,vec2(0.98,0.6),vec2(1.0,0.8-width));

    draw_rec(uv,color,blue,vec2(0.8,0.0),vec2(0.98-width,0.1-width));
    draw_rec(uv,color,blue,vec2(0.98,0.0),vec2(1.0,0.1-width));

    Frag_Color = vec4(color,1.0);
}
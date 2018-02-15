#version 430
layout(location = 1) uniform float time;
layout(location = 2) uniform vec2 resolution;
layout(location = 3) uniform vec2 mouse;
layout(location = 4)uniform vec4 fcolor;
uniform int pass;
uniform sampler2D diffuse_color;
uniform int currentID;

in vec2 tex_coord;
in vec3 normal; 
flat in int instance_id;
out vec4 fragColor;

void main()
{
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    vec2 m = mouse.xy / resolution.xy;
    m.y = 1.0 - m.y;

    vec4 color = vec4(vec3(0.0),1.0);

    if(pass == 1)
    {
        color = vec4(vec3(instance_id/255.0),1.0);
    }
    else if(pass == 2)
    {
        // clicked
        if(currentID == instance_id)
        {
           color = vec4(1.0,0.0,0.0,1.0);
           // color = texture(diffuse_color,tex_coord);
        }
        else
        {
            color = texture(diffuse_color,tex_coord);
        }
    }

    fragColor += color;
}
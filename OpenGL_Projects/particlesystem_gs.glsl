#version 430
layout(points) in;
layout(triangle_strip,max_vertices=4) out;

in vec3 pos_out[]; 
in vec3 vel_out[]; 
in float age_out[];
in vec3 camera_pos[];

out vec3 g_pos_out;
out vec3 g_vel_out;
out float g_age_out;

uniform mat4 PV;

void main()
{
    vec3 pos = gl_in[0].gl_Position.xyz;
    const float width = 0.01f;
    const float height = 1.0f;
    vec3 toCamera = normalize(camera_pos[0]-pos);
    vec3 up = vec3(0.0,1.0,0.0);
    vec3 right = cross(up,toCamera);
    vec3 tmpPos = pos;

    gl_Position = PV*vec4(pos,1.0);
    EmitVertex();

    tmpPos = pos + right * width;
    gl_Position = PV*vec4(tmpPos,1.0);
    EmitVertex();

    tmpPos = pos + vel_out[0] * height;
    gl_Position = PV*vec4(tmpPos,1.0);
    EmitVertex();

    tmpPos = pos + vel_out[0] * height + right*width;
    gl_Position = PV*vec4(tmpPos,1.0);
    EmitVertex();

    g_pos_out = pos;
    g_vel_out = vel_out[0];
    g_age_out = age_out[0];

    EndPrimitive();
}
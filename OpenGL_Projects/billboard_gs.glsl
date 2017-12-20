#version 400
layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

uniform mat4 PV;
uniform mat4 M;

in DATA_VS
{
    float id;
    vec2 tex_coord;
    vec4 norm_coord;
    vec4 pos_coord;
    vec3 camera_position;
} vs_out[];

out vec2 TexCoord;

void main()
{
    vec3 Pos = gl_in[0].gl_Position.xyz;
    vec3 toCamera = normalize(vs_out[0].camera_position - Pos);
    vec3 up = vec3(0.0,1.0,0.0);
    vec3 right = cross(toCamera, up);

    Pos -= right * 0.5;
    //gl_Position = PV*M* vec4(Pos,1.0);
    gl_Position = PV* vec4(Pos,1.0);
    TexCoord = vec2(0.0,0.0);
    EmitVertex();

    Pos.y += 1.0;
    //gl_Position = PV*M * vec4(Pos, 1.0);
    gl_Position = PV* vec4(Pos,1.0);
    TexCoord = vec2(0.0, 1.0);
    EmitVertex();

    Pos.y -= 1.0;
    Pos += right;
    //gl_Position = PV*M * vec4(Pos, 1.0);
    gl_Position = PV* vec4(Pos,1.0);
    TexCoord = vec2(1.0, 0.0);
    EmitVertex();

    Pos.y += 1.0;
    //gl_Position = PV*M * vec4(Pos, 1.0);
    gl_Position = PV* vec4(Pos,1.0);
    TexCoord = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();
}

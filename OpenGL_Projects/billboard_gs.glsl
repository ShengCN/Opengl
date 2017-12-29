#version 400
layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

uniform mat4 PV;
uniform mat4 M;
uniform float aspect;

in DATA_VS
{
    float id;
    vec2 tex_coord;
    vec4 norm_coord;
    vec4 pos_coord;
    vec3 camera_position;
} vs_out[];

out vec2 TexCoord;

const float half_width = 0.5;

void main()
{
    float half_length = half_width * aspect;

    vec3 Pos = gl_in[0].gl_Position.xyz;
    vec3 toCamera = normalize(vs_out[0].camera_position - Pos);
    vec3 up = vec3(0.0,1.0,0.0);
    vec3 right = cross(toCamera, up);
    vec3 left = -right;

    // bottom left
    Pos -= left * half_width;
    Pos.y -= half_length;
    gl_Position = PV* vec4(Pos*1.5,1.0);
    TexCoord = vec2(0.0,0.0);
    EmitVertex();

    // top left
    Pos.y += half_length * 2.0;
    gl_Position = PV* vec4(Pos*1.5,1.0);
    TexCoord = vec2(0.0, 1.0);
    EmitVertex();

    //bottom right
    Pos.y -= half_length * 2.0;
    Pos += left * half_width * 2;
    gl_Position = PV* vec4(Pos*1.5,1.0);
    TexCoord = vec2(1.0, 0.0);
    EmitVertex();

    // top right
    Pos.y += half_length * 2.0;
    gl_Position = PV* vec4(Pos*1.5,1.0);
    TexCoord = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();
}

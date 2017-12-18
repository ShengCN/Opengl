#version 400

in DATA_VS
{
    float id;
    vec2 tex_coord;
    vec4 norm_coord;
    vec4 pos_coord;
    vec3 camera_position;
} fs_in;

out vec4 FragColor;

void main()
{
    FragColor =  vec4(0.3,0.3,0.3,1.0);
}
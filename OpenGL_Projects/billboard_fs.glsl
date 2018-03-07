#version 400
uniform sampler2D texture;
uniform vec4 color;

// in DATA_VS
// {
//     float id;
//     vec2 tex_coord;
//     vec4 norm_coord;
//     vec4 pos_coord;
//     vec3 camera_position;
// } vs_in;

in vec2 TexCoord;
out vec4 FragColor;

void main()
{
    FragColor =  texture2D(texture,TexCoord);

    //FragColor = color;
    if(FragColor.r == 0 && FragColor.g == 0 && FragColor.b == 0)
    {
        discard;
    }
}
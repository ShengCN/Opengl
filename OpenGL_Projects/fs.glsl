#version 400
uniform sampler2D colorMap;

in vec2 TexCoord;
out vec4 FragColor;

uniform vec3 light_color;
void main()
{
    // FragColor =  texture2D(colorMap,TexCoord);

    // if(FragColor.r == 0 && FragColor.g == 0 && FragColor.b == 0)
    // {
    //     discard;
    // }

    FragColor = vec4(light_color,1.0);
}
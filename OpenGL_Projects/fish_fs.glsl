#version 430

uniform sampler2D diffuse_color;

out vec4 fragcolor;
in vec2 tex_coord;
in vec3 camera;
in vec3 fish_pos;
in vec3 fish_norm;

layout(location = 4) uniform vec4 color;

uniform vec3 light_position;
uniform vec3 light_color;
uniform float slider;

void main()
{
    vec4 material = texture(diffuse_color,tex_coord);

    vec3 light_effect = vec3(0.0);
    vec3 ambient = vec3(0.8);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    // phong lighting model
    vec3 light_dir =  normalize(light_position - fish_pos);
    vec3 camera_dir = normalize(camera - fish_pos);
    diffuse = max(dot(light_dir,fish_norm),0.0f) * light_color;
    
    vec3 reflect_dir = reflect(light_dir,fish_norm);
    specular = pow(max(dot(reflect_dir,camera_dir),0.0),32) * light_color;

    light_effect = ambient + diffuse + specular;
    fragcolor = vec4(light_effect * material.xyz ,1.0);
}
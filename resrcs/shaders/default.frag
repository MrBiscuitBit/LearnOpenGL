#version 330 core

in vec3 vertex_color;
in vec2 tex_coord;

uniform sampler2D texture_one;
uniform sampler2D texture_two;

out vec4 FragColor;

void main()
{
    FragColor = mix(texture(texture_one, tex_coord), texture(texture_two, tex_coord), 0.2f);
}
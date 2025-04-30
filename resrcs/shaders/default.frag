#version 330 core

in vec3 vertex_color;
in vec2 tex_coord;

uniform sampler2D texture_one;
uniform sampler2D texture_two;
uniform float mix_factor;

out vec4 FragColor;

void main()
{
    FragColor = mix(texture(texture_one, tex_coord), texture(texture_two, vec2(-tex_coord.x, tex_coord.y)), mix_factor);
}
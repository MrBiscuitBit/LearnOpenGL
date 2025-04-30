#version 330 core

in vec3 vertex_color;
in vec2 tex_coord;

uniform sampler2D our_texture;

out vec4 FragColor;

void main()
{
    FragColor = texture(our_texture, tex_coord) * vec4(vertex_color, 1.0f);
}
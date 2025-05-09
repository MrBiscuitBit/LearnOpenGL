#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex_Coord;

out vec3 vertex_color;
out vec2 tex_coord;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0f);
    vertex_color = aColor;
    tex_coord = aTex_Coord;
}
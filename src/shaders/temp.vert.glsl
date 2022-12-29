#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out vec2 tex_coords;

uniform mat4 model;
uniform mat4 camera;

void main()
{
    tex_coords = uv;
    gl_Position = camera * model * vec4(position, 1.0);
}
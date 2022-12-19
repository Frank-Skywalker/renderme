#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out vec2 tex_coords;

//uniform mat4 model;
//uniform mat4 view_projection;

void main()
{
    tex_coords = uv;
    //gl_Position = view_projection * model * vec4(aPos, 1.0);
    gl_Position = vec4(position, 1.0);
}
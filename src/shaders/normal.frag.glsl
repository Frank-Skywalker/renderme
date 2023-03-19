#version 330 core
out vec4 fragment_color;
in vec2 frag_uv;
in vec3 world_normal;

uniform sampler2D texture_diffuse0;

void main()
{
    fragment_color = vec4(world_normal,1.0f);
}
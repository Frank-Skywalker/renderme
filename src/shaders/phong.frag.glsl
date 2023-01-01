#version 330 core
out vec4 fragment_color;
in vec2 frag_uv;
in vec3 frag_normal;

uniform sampler2D texture_diffuse0;

void main()
{
    fragment_color = texture(texture_diffuse0, frag_uv);
    //fragment_color = vec4(frag_normal,1);
}
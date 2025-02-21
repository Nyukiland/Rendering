#version 410

out vec4 out_color;
in vec3 vertex_position;
in vec2 uv;

uniform sampler2D m_texture;

void main()
{
    //out_color = vec4(vertex_position, 1.);
    if (length(vec2(0.5f,0.5f) - uv) < 0.4f) out_color = vec4(0,0,0,0);
    else out_color = vec4(1.0);
}
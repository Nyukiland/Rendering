#version 410

out vec4 out_color;
in vec3 vertex_position;
in vec2 uv;

uniform sampler2D m_texture;

void main()
{
    //out_color = vec4(vertex_position, 1.);
    out_color = texture(m_texture, uv);
}
#version 410

out vec4 out_color;
in vec3 vertex_position;
in vec2 uv;

uniform sampler2D m_texture;

void main()
{
    //out_color = vec4(vertex_position, 1.);
    if (uv.x < 0.5f && uv.y < 0.5f) out_color = vec4(0,0,0,1);
    else if (uv.x > 0.5f && uv.y > 0.5f) out_color = vec4(0,0,0,1);
    else out_color = vec4(1.0, 0.0, 0.8, 1.0);
}
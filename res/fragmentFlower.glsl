#version 410

out vec4 out_color;
in vec3 vertex_position;
in vec2 uv;
in vec3 normal;

uniform sampler2D m_texture;
uniform vec3 lightDir;

void main()
{
    //a + (b - a) * t
    float t = normal.z * -1;
    
    vec4 colora = vec4(0.83, 0.0, 0.69, 1.0);
    vec4 colorb = vec4(0.27, 1.0, 0.0, 1.0);
    vec4 colorf = colora + (colorb - colora) * t;

    float dotValue = dot(normal, lightDir);
    dotValue = clamp(dotValue, 0.3, 1);
    
    out_color = vec4(colorf.xyz * dotValue, 1.0) + vec4(0.1,0.1,0.1,1);
}
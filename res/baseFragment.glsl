#version 410

out vec4 out_color;

uniform vec3 ColorGive;

void main()
{
    vec3 color = ColorGive;
    out_color = vec4(color, 0.05);
}
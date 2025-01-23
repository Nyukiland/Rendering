#version 410

layout(location = 0) in vec2 in_position;

uniform vec2 PositionChange;

void main()
{
    vec2 pos = in_position;
    pos += PositionChange;
    gl_Position = vec4(pos, 0., 1.);
}
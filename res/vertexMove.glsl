#version 410

layout(location = 0) in vec2 in_position;

uniform mat4 Project;

void main()
{
    gl_Position = Project * vec4(in_position, 0, 1.);
}
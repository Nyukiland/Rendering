#version 410

layout(location = 0) in vec3 in_position;

uniform mat4 Project;

out vec3 vertex_position;

void main()
{
    vertex_position = in_position;
    gl_Position = Project * vec4(in_position, 1.);
}
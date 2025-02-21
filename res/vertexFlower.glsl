#version 410

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

uniform mat4 Project;

out vec3 vertex_position; 
out vec2 uv;
out vec3 normal;

void main()
{
    uv = in_uv;
    normal = in_normal;
    vertex_position = in_position;
    gl_Position = Project * vec4(in_position, 1.);
}
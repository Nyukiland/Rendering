#version 410

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

uniform mat4 Project;
uniform mat4 TransformMatrix;

out vec3 vertex_position; 
out vec2 uv;
out vec3 normal;

vec3 apply_matrix_to_position(mat4 matrix, vec3 point)
{
    vec4 tmp = matrix * vec4(point, 1.);
    return tmp.xyz / tmp.w;
}

vec3 apply_matrix_to_direction(mat4 matrix, vec3 direction)
{
    vec4 tmp = matrix * vec4(direction, 0.);
    return normalize(tmp.xyz);
}

void main()
{
    //considering that the rotation we do is the camera and not the object itself
    uv = in_uv;
    normal = apply_matrix_to_direction(TransformMatrix, in_normal);
    vertex_position = apply_matrix_to_position(TransformMatrix, in_position);
    gl_Position = Project * vec4(in_position, 1.);
}
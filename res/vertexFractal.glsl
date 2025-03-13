#version 410

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

uniform mat4 Project;
uniform mat4 TransformMatrix;

out vec3 vertex_position_os; 
out vec3 vertex_position_ws; 
out vec3 normal_os;
out vec3 normal_ws;

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
    normal_os = in_normal;
    //normal_ws = apply_matrix_to_direction(glm::inverse(glm::transpose(TransformMatrix)), in_normal);
    normal_ws = apply_matrix_to_direction(TransformMatrix, in_normal);
    
    vertex_position_os = in_position;
    vertex_position_ws = apply_matrix_to_position(TransformMatrix, in_position);
    gl_Position = Project * vec4(in_position, 1.);
}
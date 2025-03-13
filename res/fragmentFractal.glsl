#version 410

out vec4 out_color;
in vec3 vertex_position_os;
in vec3 vertex_position_ws;
in vec3 normal_os;
in vec3 normal_ws;

void main()
{
    //lerp
    //a + (b - a) * t
    
    // Base color for the material (e.g., a purple color)
    float t = clamp(vertex_position_os.z, 0, 1);
    
    vec4 colora = vec4(0.83, 0.0, 0.69, 1.0);
    vec4 colorb = vec4(0.27, 1.0, 0.0, 1.0);
    vec4 colorf = colora + (colorb - colora) * t;


    if (abs(normal_os.y) > 0.5) { 
        if (normal_os.y > 0.0) {
            colorf = vec4(0.0, 0.0, 1.0, 1.0); //top 
        } else {
            colorf = vec4(1.0, 0.0, 0.0, 1.0); //bottom 
        }
    } 
    else if (abs(normal_os.x) > 0.5) {
        if (normal_os.x > 0.0) {
            colorf = vec4(0.0, 1.0, 1.0, 1.0); //right
        } else {
            colorf = vec4(1.0, 1.0, 0.0, 1.0); //left
        }
    }
    else if (abs(normal_os.z) > 0.5) {
        if (normal_os.z > 0.0) {
            colorf = vec4(1.0, 0.5, 0.0, 1.0); //front 
        } else {
            colorf = vec4(0.5, 0.0, 1.0, 1.0); //back
        }
    }
    else if (length(normal_os) < 0.0001f)
    {
        colorf = vec4(0.19, 0.0, 0.38, 1.0);
    }

    //out_color = colorf;
    out_color = vec4(abs(normal_os.x), abs(normal_os.y), abs(normal_os.z), 1.0);
}
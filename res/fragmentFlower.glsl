#version 410

out vec4 out_color;
in vec3 vertex_position_os;
in vec3 vertex_position_ws;
in vec2 uv;
in vec3 normal_os;
in vec3 normal_ws;

uniform sampler2D m_texture;
uniform vec3 lightDir;
uniform vec4 lightColor;

uniform vec3 pointLight;
uniform float lightIntensity;
uniform vec4 pointLightColor;

void main()
{
    //lerp
    //a + (b - a) * t
    
    // Base color for the material (e.g., a purple color)
    vec4 colora = vec4(0.35, 0.0, 1.0, 1.0);
    //vec4 colorb = vec4(0.27, 1.0, 0.0, 1.0);
    //vec4 colorf = colora + (colorb - colora) * t;

    // Dir light 
    float dotValue = dot(normal_ws, lightDir);
    float shadowDot = clamp(dotValue, 0.3, 1.0);
    vec4 colorToGive = vec4(colora.xyz * shadowDot, 1.0) + vec4(lightColor.xyz * clamp(dotValue, 0.0, 1.0), 1.0);
    
    // Point light
    vec3 fragToLight = pointLight - vertex_position_ws;
    float distance = length(fragToLight); 
    float attenuation = lightIntensity / (distance * distance);
    attenuation = clamp(attenuation, 0.0, 1.0);
    
    //combine and clamp
    colorToGive += pointLightColor * attenuation;
    colorToGive = clamp(colorToGive, 0.0, 1.0);

    out_color = colorToGive;
}
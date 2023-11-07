#version 460 core

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform bool isLighting;

in vec3 Normal;
in vec3 fragPos;

void main()
{   
    vec3 result;
    if (isLighting)
    {
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - fragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
    
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lightColor;
        result = (ambient + diffuse) * objectColor;
    }
    else
    {
        result = objectColor;
    }
    color = vec4(result, 1.0);
}
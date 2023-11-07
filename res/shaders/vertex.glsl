#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 fragPos;

void main() 
{
    mat4 mvp = projection * view * model;
    mat3 normalMatrix = mat3(mvp);
    normalMatrix = inverse(normalMatrix);
    normalMatrix = transpose(normalMatrix);
    Normal = normalize(normal * normalMatrix);

    gl_Position = mvp * vec4(position, 1.0);
    fragPos = vec3(mvp * vec4(position, 1.0));
}
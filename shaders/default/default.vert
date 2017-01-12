#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vTexCoord;
layout (location = 3) in float vType;
layout (location = 4) in vec3 vNormal;

out vec3 Position;
out vec3 Color;
out vec2 TexCoord;
out float Type;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(vPosition, 1.0f);

    Position = vec3(model * vec4(vPosition, 1.0f));
    Color=vColor;
    TexCoord=vTexCoord;
    Type=vType;
    Normal = mat3(transpose(inverse(model))) * vNormal;
}

#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 5) in ivec4 aJointID;
layout (location = 6) in vec4 aJointWeights;

out vec2 TexCoords;
out vec3 Normals;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * boned_position);
    TexCoords = aTexCoords;
    gl_Position = projection * view * model;
}
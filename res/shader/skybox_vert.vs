#version 410 core
layout (location = 0) in vec3 sk_Pos;

out vec3 sk_TexCoords;

uniform mat4 sk_projection;
uniform mat4 sk_view;

void main()
{
    sk_TexCoords = sk_Pos;
    vec4 sk_pos = sk_projection * sk_view * vec4(sk_Pos, 1.0);
    gl_Position = sk_pos.xyww;
}  
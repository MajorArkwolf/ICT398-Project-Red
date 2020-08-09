#version 410 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normals;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform sampler2D texture1;

void main()
{             
    FragColor = texture(texture1, TexCoords) * vec4(result, 1.0);
}

#version 410 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normals;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform bool has_texture;
uniform bool has_color;
uniform vec4 color_value;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main()
{
    vec4 color;
    if (has_texture) {
        color = texture(texture_diffuse1, TexCoords);
    } else if (has_color) {
        color = color_value;
    } else {
        color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    }
    // ambient
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normals);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.05;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular);
    FragColor = color * vec4(result, 1.0);
}

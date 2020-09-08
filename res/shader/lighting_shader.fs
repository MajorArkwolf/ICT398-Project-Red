#version 410 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normals;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

//Hacked in
in vec2 TexCoords;
uniform sampler2D texture_diffuse1;
uniform bool has_texture;

void main()
{
    // Start of hack
    vec4 color;
    if (has_texture) {
        color = texture(texture_diffuse1, TexCoords);
    } else {
        color = vec4(0.5f, 0.5f, 0.5f, 0.5f);
    }
    //End of hack
    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 norm = normalize(Normals);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0) + color;
}
#version 410 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D texture_diffuse1;
uniform bool has_texture;
uniform bool has_color;
uniform vec4 color_value;

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
    FragColor = color;
}

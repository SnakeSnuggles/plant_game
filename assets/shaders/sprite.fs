#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texturea;
in vec3 ourColor;

void main() {
    FragColor = texture(texturea, TexCoords) * vec4(ourColor,1.0);
}

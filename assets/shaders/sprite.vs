#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoord;


out vec2 TexCoords;
uniform mat4 transforma;
uniform mat4 projection;

void main() {
    gl_Position = transforma * vec4(aPos, 1.0f);
    // gl_Position = vec4(aPos, 1.0f); 
    TexCoords = aTexCoord;
}
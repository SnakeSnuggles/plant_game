#pragma once
#include "Shader.hpp"
#include "Texture.hpp"
#include "VBO.hpp"
#include "VAO.hpp"
#include "../vars.hpp"

class Sprite {
public:
    Shader shader{"shaders/sprite.vs", "shaders/sprite.fs"};
    Texture texture;
    float vertices[48];
    VBO vbo;
    VAO vao;
    float x, y, z;
    float width, height;

    Sprite(const char* path, float scale, float xi, float yi, float zi)
        : texture(path, true),
          vbo(nullptr, sizeof(vertices), GL_DYNAMIC_DRAW), // We'll fill vertices after init
          vao(vbo),
          x(xi), y(yi), z(zi)
    {
        width = static_cast<float>(texture.width) / WIDTH * scale;
        height = static_cast<float>(texture.height) / HEIGHT * scale;
        init_geometry();
    }

    void init_geometry() {
        float temp_vertices[48] = {
            // First Triangle
            x,          y,          z,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,  // Bottom-left
            x + width,  y,          z,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,  // Bottom-right
            x,          y + height, z,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // Top-left

            // Second Triangle
            x,          y + height, z,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // Top-left
            x + width,  y,          z,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f,  // Bottom-right
            x + width,  y + height, z,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f   // Top-right
        };

        std::copy(std::begin(temp_vertices), std::end(temp_vertices), std::begin(vertices));
        vbo.bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    }

    void updateVBO() {
        float temp_vertices[48] = {
            x, y, z, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            x + width, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            x, y + height, z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            x, y + height, z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            x + width, y, z, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            x + width, y + height, z, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
        };

        std::memcpy(vertices, temp_vertices, sizeof(temp_vertices));
        vbo.bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    }
};

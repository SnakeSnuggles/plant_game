#pragma once
#include "primitives/Shader.hpp"
#include "primitives/Texture.hpp"
#include "primitives/VBO.hpp"
#include "primitives/VAO.hpp"
#include "vars.hpp"

class Sprite;

class Sprite_Manager {
    public:
        static std::vector<Sprite*> sprites;
        static void draw();
};

class Sprite {
public:
    Shader shader{"shaders/sprite.vs", "shaders/sprite.fs"};
    Texture texture;
    float vertices[48];  // Correctly store vertex data at the class level
    VBO vbo;
    VAO vao;
    float x,y,z;
    float width, height;

    Sprite(const char* path, float scale, float xi, float yi, float zi) 
        : texture(path, true),
          vbo(vertices, sizeof(vertices), GL_DYNAMIC_DRAW),
          vao(vbo) 
    {
        x = xi;
        y = yi;
        z = zi;
        width = (float)texture.width / WIDTH * scale;
        height = (float)texture.height / HEIGHT * scale;

        // Set proper vertex positions for a rectangle
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

        // Copy temp_vertices into the class member
        std::copy(std::begin(temp_vertices), std::end(temp_vertices), std::begin(vertices));

        // Now initialize VBO and VAO after setting vertices

        Sprite_Manager::sprites.push_back(this);
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
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(temp_vertices), vertices);
    }

    void draw() {
        shader.use();
        glActiveTexture(GL_TEXTURE0 + texture.ID);
        texture.bind();
        shader.setInt("texturea", texture.ID);
        vao.draw();
    }
};


#pragma once
#include "Shader.hpp"
#include "Texture.hpp"
#include "VBO.hpp"
#include "VAO.hpp"
#include "../vars.hpp"
#include "trigonometric.hpp"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>
class Sprite {
public:
    Shader shader{"shaders/sprite.vs", "shaders/sprite.fs"};
    Texture texture;
    float vertices[48];
    VBO vbo;
    VAO vao;
    float x = 0.0f, y = 0.0f, z = 0.0f;
    glm::vec3 rotation{0,0,0};
    float width, height;

    Sprite(const char* path, float scale)
        : texture(path, true),
          vbo(nullptr, sizeof(vertices), GL_DYNAMIC_DRAW), 
          vao(vbo)
    {
        width = static_cast<float>(texture.width) / WIDTH * scale;
        height = static_cast<float>(texture.height) / HEIGHT * scale;
        init_geometry();
        set_rotation(rotation);
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
    void set_rotation(glm::vec3 rotation) {
        
        this->rotation = rotation;
    }
    void retateions() {

        glm::vec3 radians = glm::radians(rotation);
        
        glm::mat4 transform = glm::mat4(1.0f);

        transform = glm::rotate(transform, radians.x, glm::vec3(1, 0, 0));  // X axis
        transform = glm::rotate(transform, radians.y, glm::vec3(0, 1, 0));  // Y axis
        transform = glm::rotate(transform, radians.z, glm::vec3(0, 0, 1));  // Z axis
        glm::mat4 projection = glm::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f); 
        shader.setMat4("transforma", transform);
        // shader.setMat4("projection", projection);
        // std::cout << "Projection Matrix: " << glm::to_string(projection) << std::endl;
        std::cout << "Transform Matrix: " << glm::to_string(transform) << std::endl;
    }

};

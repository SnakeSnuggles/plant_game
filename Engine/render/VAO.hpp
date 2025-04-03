#pragma once
#include "../cludes.hpp"

class VAO {
    public:
        unsigned int vao_id;
        size_t size;

        VAO(VBO vbo) {
            size = vbo.size;
            glGenVertexArrays(1, &vao_id);
            glBindVertexArray(vao_id);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // Position
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Color
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // Texture Coords
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(0);
        }

        void draw() {
            glBindVertexArray(vao_id);
            glDrawArrays(GL_TRIANGLES, 0, size);
        }
};

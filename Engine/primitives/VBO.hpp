#pragma once
#include "../cludes.hpp"
class VBO {
    public:
        float* vertices;
        size_t size;
        unsigned int vbo_id;
        unsigned int draw_type;

        VBO(float verts[], size_t s,unsigned int type) {
            size = s;
            vertices = new float[size / sizeof(float)];
            draw_type = type;
            std::memcpy(vertices, verts, size);

            glGenBuffers(1, &vbo_id);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
            glBufferData(GL_ARRAY_BUFFER, size, vertices, draw_type);
        }

        ~VBO() {
            delete[] vertices;
        }

        operator unsigned int() const {
            return vbo_id;
        }
};

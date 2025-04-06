#pragma once
#include "../cludes.hpp"

class VBO {
    public:
        float* vertices;
        size_t size;
        unsigned int vbo_id;
        unsigned int draw_type;

        VBO(float verts[], size_t s, unsigned int type)
            : vertices(nullptr), size(s), draw_type(type) {
        
            vertices = new float[size / sizeof(float)];
            
            if (verts != nullptr) {
                std::memcpy(vertices, verts, size);
            } else {
                std::memset(vertices, 0, size); // optional
            }
        
            glGenBuffers(1, &vbo_id);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
            glBufferData(GL_ARRAY_BUFFER, size, vertices, draw_type);
        }
        void bind() const {
            glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        }
        ~VBO() {
            delete[] vertices;
        }

        operator unsigned int() const {
            return vbo_id;
        }
};

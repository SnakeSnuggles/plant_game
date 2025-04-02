#pragma once 
#include "../cludes.hpp"

class Texture_Manager {
    public:
        static int current_texture_id;

        static int get() {
            return current_texture_id++;
        }
};


class Texture {
public:
    unsigned int ID = Texture_Manager::get();
    int width, height, nrChannels;

    Texture(std::string path="error.png", bool alpha = false) {
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        // Set texture wrapping/filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load image with stb_image
        stbi_set_flip_vertically_on_load(false); // Flip to match OpenGL's texture coordinate system
        unsigned char* data = stbi_load((asset_path+path).c_str(), &width, &height, &nrChannels, 0);

        if (data) {
            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            
            // Generate texture with proper format
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            // Enable transparency by setting blending function
            if (alpha) {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
        } else {
            std::cerr << "Failed to load texture: " << path << std::endl;
        }

        // Free image memory
        stbi_image_free(data);
    }

    // Bind texture for use
    void bind() const {
        glBindTexture(GL_TEXTURE_2D, ID);
    }

    ~Texture() {
        glDeleteTextures(1, &ID);
    }
};


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstring>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// Window size vars
unsigned int WIDTH = 800;
unsigned int HEIGHT = 600;
std::string asset_path = "assets/";

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

class Shader {
public:
    unsigned int ID;

    Shader(std::string vertexPath, std::string fragmentPath) {
        std::string vertexCode, fragmentCode;
        try {
            vertexCode = readFile((asset_path + vertexPath).c_str());
            fragmentCode = readFile((asset_path + fragmentPath).c_str());
        } catch (const std::exception& e) {
            std::cerr << "ERROR::SHADER::FILE_READ_FAILED\n" << e.what() << std::endl;
            return;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertex = compileShader(vShaderCode, GL_VERTEX_SHADER);
        unsigned int fragment = compileShader(fShaderCode, GL_FRAGMENT_SHADER);

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use() const {
        glUseProgram(ID);
    }

    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    ~Shader() {
        glDeleteProgram(ID);
    }

private:
    std::string readFile(const char* path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open shader file: " + std::string(path));
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    unsigned int compileShader(const char* code, GLenum type) {
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &code, nullptr);
        glCompileShader(shader);
        checkCompileErrors(shader, (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT");
        return shader;
    }

    void checkCompileErrors(unsigned int shader, const std::string& type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << "\n";
            }
        }
    }
};


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



int Texture_Manager::current_texture_id = 0;


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

std::vector<Sprite*> Sprite_Manager::sprites;

void Sprite_Manager::draw() {
    std::sort(Sprite_Manager::sprites.begin(), Sprite_Manager::sprites.end(), [](Sprite* a, Sprite* b) {
        return a->z < b->z;  // Lower z first
    });

    for(auto sprite : Sprite_Manager::sprites) {
        sprite->draw();
        sprite->updateVBO();
    }
    for(auto sprite : Sprite_Manager::sprites) {
        sprite->draw();
        sprite->updateVBO();
    }
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    HEIGHT = height;
    WIDTH = width;
}

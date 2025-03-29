#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstring>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// Window size vars
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

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

    Shader(const char* vertexPath, const char* fragmentPath) {
        std::string vertexCode, fragmentCode;
        try {
            vertexCode = readFile(vertexPath);
            fragmentCode = readFile(fragmentPath);
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

class Texture {
public:
    unsigned int ID;
    int width, height, nrChannels;

    Texture(const char* path, bool alpha = false) {
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        // Set texture wrapping/filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load image with stb_image
        stbi_set_flip_vertically_on_load(true); // Flip to match OpenGL's texture coordinate system
        unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

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

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float vertices[] = {
    // First triangle
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,

    // Second triangle
   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f
    };

    Shader shader{"shaders/vertex.vs", "shaders/fragment.fs"};
    VBO triangle{vertices, sizeof(vertices), GL_STATIC_DRAW};  
    VAO rect{triangle};
    Texture texture{"char.png", true};

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        texture.bind();
        shader.setInt("texture1", 0);  // Use texture unit 0

        rect.draw();  // Draw the full rectangle (6 vertices)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

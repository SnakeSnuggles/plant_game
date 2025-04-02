#include "vars.hpp"
#include "Sprite.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int Texture_Manager::current_texture_id = 0;

class App {
private:
    GLFWwindow* window;
    int width, height;

public:
    App() : window(nullptr), width(800), height(600) {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, "Plant Game", nullptr, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return;
        }

        glViewport(0, 0, width, height);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    }

    int run() {
        if (!window) return -1;

        while (!glfwWindowShouldClose(window)) {
            processInput(window);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            Sprite_Manager::draw();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }

    static void framebuffer_size_callback(GLFWwindow* window, int newWidth, int newHeight) {
        glViewport(0, 0, newWidth, newHeight);
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        if (app) {
            app->width = newWidth;
            app->height = newHeight;
        }
    }
};

std::vector<Sprite*> Sprite_Manager::sprites;

void Sprite_Manager::draw() {
    std::sort(Sprite_Manager::sprites.begin(), Sprite_Manager::sprites.end(), [](Sprite* a, Sprite* b) {
        return a->z < b->z;
    });

    for (auto sprite : Sprite_Manager::sprites) {
        if (!sprite) continue;
        sprite->draw();
        sprite->updateVBO();
    }
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

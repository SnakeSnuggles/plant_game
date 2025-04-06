#pragma once
#include "render/Sprite.hpp"
#include "ecs/ecs.hpp"
#include "ecs/default_system.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


int Texture_Manager::current_texture_id = 0;

class App {
private:
    GLFWwindow* window;
    int width, height;

public:
    App(std::string window_name) : window(nullptr), width(800), height(600) {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, window_name.c_str(), nullptr, nullptr);
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

        float last_time = glfwGetTime(); 
        
        while (!glfwWindowShouldClose(window)) {
            float current_time = glfwGetTime();
            float delta_time = current_time - last_time;
            last_time = current_time;

            processInput(window);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);


            System_Manager::update(delta_time);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }
    GLFWwindow* get_window() {
        return window;
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

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

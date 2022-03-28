//
// Created by smiga287 on 3/27/22.
//

#include "GlfwWindow.h"
#include <iostream>
#include "GlfwWindowBuilder.h"

GlfwWindow::GlfwWindow(int width, int height, const std::string& title) {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    window_ = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window_) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(1);
    }
    glfwMakeContextCurrent(window_);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

GLFWwindow* GlfwWindow::get_raw_window() const {
    return window_;
}

GlfwWindow::~GlfwWindow() {
    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
}

void GlfwWindow::swapBuffers() {
    glfwSwapBuffers(window_);
}

void GlfwWindow::pollEvents() {
    glfwPollEvents();
}

bool GlfwWindow::shouldClose() {
    return glfwWindowShouldClose(window_);
}

void GlfwWindow::setShouldClose(bool value) {
    glfwSetWindowShouldClose(window_, value);
}

bool GlfwWindow::isKeyPressed(int key) const {
    return glfwGetKey(window_, key) == GLFW_PRESS;
}

GlfwWindowBuilder GlfwWindow::create(int width, int height, const std::string &title) {
    return GlfwWindowBuilder{width, height, title};
}

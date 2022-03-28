//
// Created by smiga287 on 3/27/22.
//

#ifndef FUTURAMA_GLFWWINDOWBUILDER_H
#define FUTURAMA_GLFWWINDOWBUILDER_H

#include "GlfwWindow.h"
#include <string>

class GlfwWindowBuilder {
public:
    GlfwWindowBuilder(int width, int height, const std::string& title) : window_(width, height, title) {}

    template<class Callback>
    GlfwWindowBuilder& setFramebufferSizeCallback(Callback cb) {
        glfwSetFramebufferSizeCallback(window_.window(), cb);
        return *this;
    }
    template<class Callback>
    GlfwWindowBuilder& setMouseCallback(Callback cb) {
        glfwSetCursorPosCallback(window_.window(), cb);
        return *this;
    }
    template<class Callback>
    GlfwWindowBuilder& setScrollCallback(Callback cb) {
        glfwSetScrollCallback(window_.window(), cb);
        return *this;
    }
    template<class Callback>
    GlfwWindowBuilder& setKeyCallback(Callback cb) {
        glfwSetKeyCallback(window_.window(), cb);
        return *this;
    }

    // Implicit cast operator so that the user never needs to know they interact with a builder
    operator GlfwWindow() const { return window_; }
private:
    GlfwWindow window_;
};


#endif //FUTURAMA_GLFWWINDOWBUILDER_H

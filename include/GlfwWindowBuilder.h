//
// Created by smiga287 on 3/27/22.
//

#ifndef FUTURAMA_GLFWWINDOWBUILDER_H
#define FUTURAMA_GLFWWINDOWBUILDER_H

#include "GlfwWindow.h"
#include <string>

class GlfwWindowBuilder {
public:
  GlfwWindowBuilder(int width, int height, const std::string &title)
      : window_(width, height, title) {}

  template <class Callback>
  GlfwWindowBuilder &setFramebufferSizeCallback(Callback cb) {
    glfwSetFramebufferSizeCallback(window_.get_raw_window(), cb);
    return *this;
  }
  template <class Callback> GlfwWindowBuilder &setMouseCallback(Callback cb) {
    glfwSetCursorPosCallback(window_.get_raw_window(), cb);
    return *this;
  }
  template <class Callback> GlfwWindowBuilder &setScrollCallback(Callback cb) {
    glfwSetScrollCallback(window_.get_raw_window(), cb);
    return *this;
  }
  template <class Callback> GlfwWindowBuilder &setKeyCallback(Callback cb) {
    glfwSetKeyCallback(window_.get_raw_window(), cb);
    return *this;
  }

  GlfwWindow *build() {
    // We load glad only when we've already set up all of the callbacks
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cerr << "Failed to initialize GLAD" << std::endl;
      std::exit(1);
    }

    return &window_;
  }

private:
  GlfwWindow window_;
};

#endif // FUTURAMA_GLFWWINDOWBUILDER_H

//
// Created by smiga287 on 3/27/22.
//

#ifndef FUTURAMA_GLFWWINDOW_H
#define FUTURAMA_GLFWWINDOW_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <memory>
#include <string>

class GlfwWindowBuilder;

class GlfwWindow {
public:
  static GlfwWindowBuilder create(int width, int height,
                                  const std::string &title);

  void swapBuffers();
  void pollEvents();
  bool shouldClose();
  void setShouldClose(bool value);

  bool isKeyPressed(int key) const;

  GLFWwindow *get_raw_window() const;
  ~GlfwWindow();

public: // TODO: change to protected
  GlfwWindow(int width, int height, const std::string &title);

private:
  GLFWwindow *window_;
  friend class GlfwWindowBuilder;
};

#endif // FUTURAMA_GLFWWINDOW_H

//
// Created by smiga287 on 3/28/22.
//

#ifndef FUTURAMA_PROGRAM_STATE_H
#define FUTURAMA_PROGRAM_STATE_H

#include <string>
#include <learnopengl/camera.h>

// settings
const uint SCR_WIDTH = 800;
const uint SCR_HEIGHT = 600;

struct PointLight {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct ProgramState {
    glm::vec3 clearColor = glm::vec3(0);
    bool ImGuiEnabled = false;
    Camera camera;
    bool CameraMouseMovementUpdateEnabled = true;
    glm::vec3 backpackPosition = glm::vec3(0.0f);
    float backpackScale = 1.0f;
    PointLight pointLight;

    bool blinn = false;
    bool flashLight = false;
    glm::vec3 pointLightSource = glm::vec3{-6.0f, 0.2f, -5.6f};

    // camera
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;

    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    ProgramState()
            : camera(glm::vec3(0.0f, 0.0f, 3.0f)),
              pointLight{
                  .position = glm::vec3(4.0f, 4.0, 0.0),
                  .ambient = glm::vec3(0.1, 0.1, 0.1),
                  .diffuse = glm::vec3(0.8, 0.8, 0.8),
                  .specular = glm::vec3(1.0, 1.0, 1.0),
                  .constant = 1.0f,
                  .linear = 0.05f,
                  .quadratic = 0.012f
              } {}

    void SaveToFile(std::string filename);

    void LoadFromFile(std::string filename);
};

inline void ProgramState::SaveToFile(std::string filename) {}

inline void ProgramState::LoadFromFile(std::string filename) {}

// Move declarations from main
struct GLFWwindow;
class GlfwWindow;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GlfwWindow *window);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

#endif //FUTURAMA_PROGRAM_STATE_H

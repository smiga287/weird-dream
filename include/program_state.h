//
// Created by smiga287 on 3/28/22.
//

#ifndef FUTURAMA_PROGRAM_STATE_H
#define FUTURAMA_PROGRAM_STATE_H

#include <string>
#include <learnopengl/camera.h>


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
    ProgramState()
            : camera(glm::vec3(0.0f, 0.0f, 3.0f)) {}

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

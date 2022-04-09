//
// Created by smiga287 on 3/28/22.
//

#ifndef FUTURAMA_PROGRAM_STATE_H
#define FUTURAMA_PROGRAM_STATE_H

#include <string>
#include <learnopengl/camera.h>
#include <fstream>

// settings
const uint SCR_WIDTH = 800;
const uint SCR_HEIGHT = 600;
const float aspect = (float) SCR_WIDTH / SCR_HEIGHT;

struct PointLight {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

// Caches data which should be calculated only once per frame
struct FrameMemo {
    glm::mat4 projection;
    glm::mat4 view;
};

struct ProgramState {
    glm::vec3 clearColor = glm::vec3(0);
    bool is_ImGui_enabled = false;
    bool is_wireframe_enabled = false;
    bool is_camera_mouse_movement_enabled = true;
    Camera camera;
    glm::vec3 building_position = glm::vec3(0.0f);
    float building_scale = 1.0f;
    PointLight pointLight;

    bool is_blinn_enabled = false;
    bool is_flashlight_enabled = true;
    glm::vec3 pointLightSource = glm::vec3{-6.0f, 0.2f, -5.6f};

    // camera
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool is_first_mouse = true;

    FrameMemo frame_memo;

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

    void saveToFile(const std::string &filename) const;

    void loadFromFile(const std::string &filename);
};

// TODO: Refactor
const char SEPARATOR = '\n';

// TODO: figure out what to do with program_state.txt
inline void ProgramState::saveToFile(const std::string &filename) const {
    auto out = std::ofstream{filename};
    out << is_ImGui_enabled << SEPARATOR
        << is_wireframe_enabled << SEPARATOR
        << is_camera_mouse_movement_enabled << SEPARATOR
        << camera.Position.x << SEPARATOR
        << camera.Position.y << SEPARATOR
        << camera.Position.z << SEPARATOR
        << camera.Front.x << SEPARATOR
        << camera.Front.y << SEPARATOR
        << camera.Front.z << SEPARATOR
        << building_scale << SEPARATOR
        << building_position.x << SEPARATOR
        << building_position.y << SEPARATOR
        << building_position.z << SEPARATOR;
}

inline void ProgramState::loadFromFile(const std::string &filename) {
    auto in = std::ifstream{filename};
    in  >> is_ImGui_enabled
        >> is_wireframe_enabled
        >> is_camera_mouse_movement_enabled
        >> camera.Position.x
        >> camera.Position.y
        >> camera.Position.z
        >> camera.Front.x
        >> camera.Front.y
        >> camera.Front.z
        >> building_scale
        >> building_position.x
        >> building_position.y
        >> building_position.z;
}

// Move declarations from main
struct GLFWwindow;
class GlfwWindow;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GlfwWindow *window);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

#endif //FUTURAMA_PROGRAM_STATE_H

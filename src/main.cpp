#include "imgui_impl_glfw.h"
#include "Skybox.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include <GlfwWindow.h>
#include <GlfwWindowBuilder.h>
#include <memory>
#include <ImGuiFacade.h>
#include <program_state.h>
#include <Floor.h>
#include <drawing.h>

std::unique_ptr<ProgramState> programState;

int main() {
    auto builder = GlfwWindow::create(SCR_WIDTH, SCR_HEIGHT, "Futurama");
    GlfwWindow* window = builder
            .setFramebufferSizeCallback(framebuffer_size_callback)
            .setMouseCallback(mouse_callback)
            .setScrollCallback(scroll_callback)
            .setKeyCallback(key_callback)
            .build();

    programState = std::make_unique<ProgramState>();
    programState->loadFromFile("resources/program_state.txt");

    auto imGui = ImGuiFacade{window};

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    auto skybox_shader = Shader{"resources/shaders/skybox.vs", "resources/shaders/skybox.fs"};
    auto lights_shader = Shader{"resources/shaders/lights.vs", "resources/shaders/lights.fs"};

    // load models
    auto building_model = Model{"resources/objects/building/Home_work5.fbx"};
    building_model.SetShaderTextureNamePrefix("material.");

    // setup
    auto skybox = Skybox{skybox_shader};
    auto floor = Floor{lights_shader};

    // load textures
    skybox.loadTextures();
    floor.loadTextures();

    while (!window->shouldClose()) {
        // per-frame time logic
        float currentFrame = glfwGetTime();
        programState->deltaTime = currentFrame - programState->lastFrame;
        programState->lastFrame = currentFrame;

        processInput(window);

        // render
        glClearColor(programState->clearColor.r, programState->clearColor.g, programState->clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // lights shader setup
        setup_lighting(lights_shader, *programState);

        // update lighting
        programState->pointLight.position = glm::vec3(4.0 * cos(currentFrame), 4.0f, 4.0 * sin(currentFrame));

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(programState->camera.Zoom), aspect, 0.1f, 100.0f);
        glm::mat4 view = programState->camera.GetViewMatrix();
        programState->frame_memo.projection = projection;
        programState->frame_memo.view = view;

        // render everything
        draw_building(building_model, lights_shader, *programState);
        draw_skybox(skybox, *programState);
        draw_floor(floor, *programState);

        if (programState->is_ImGui_enabled) {
            imGui.draw(programState.get());
        }

        if (programState->is_wireframe_enabled) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        window->swapBuffers();
        window->pollEvents();
    }

    programState->saveToFile("resources/program_state.txt");
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GlfwWindow *window) {
    if (window->isKeyPressed(GLFW_KEY_ESCAPE))
        window->setShouldClose(true);

    auto& deltaTime = programState->deltaTime;

    if (window->isKeyPressed(GLFW_KEY_W))
        programState->camera.ProcessKeyboard(FORWARD, deltaTime);
    if (window->isKeyPressed(GLFW_KEY_S))
        programState->camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (window->isKeyPressed(GLFW_KEY_A))
        programState->camera.ProcessKeyboard(LEFT, deltaTime);
    if (window->isKeyPressed(GLFW_KEY_D))
        programState->camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    auto& firstMouse = programState->is_first_mouse;
    auto& lastX = programState->lastX;
    auto& lastY = programState->lastY;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if (programState->is_camera_mouse_movement_enabled) {
        programState->camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    programState->camera.ProcessMouseScroll(yoffset);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        programState->is_ImGui_enabled = !programState->is_ImGui_enabled;
        if (programState->is_ImGui_enabled) {
            programState->is_camera_mouse_movement_enabled = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            programState->is_camera_mouse_movement_enabled = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
}

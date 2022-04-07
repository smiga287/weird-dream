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
    programState->LoadFromFile("resources/program_state.txt");

    auto imGui = ImGuiFacade{window};

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    Shader skybox_shader("resources/shaders/skybox.vs", "resources/shaders/skybox.fs");
    Shader lights_shader("resources/shaders/lights.vs", "resources/shaders/lights.fs");

    // load models
    Model ourModel("resources/objects/backpack/backpack.obj");
    ourModel.SetShaderTextureNamePrefix("material.");

    PointLight& pointLight = programState->pointLight;
    auto& camera = programState->camera;

    // setup
    auto skybox = Skybox{skybox_shader};
    auto floor = Floor{lights_shader};

    // load textures
    skybox.loadTextures();
    floor.loadTextures();

    // draw in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    while (!window->shouldClose()) {
        // per-frame time logic
        float currentFrame = glfwGetTime();
        programState->deltaTime = currentFrame - programState->lastFrame;
        programState->lastFrame = currentFrame;

        // input
        processInput(window);

        // render
        glClearColor(programState->clearColor.r, programState->clearColor.g, programState->clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // lights shader setup
        {
            lights_shader.use();
            lights_shader.setVec3("viewPos", camera.Position);
            lights_shader.setFloat("material.shininess", 16.0f); // TODO: try out 32.0f
            lights_shader.setInt("blinn", programState->blinn);
            lights_shader.setInt("flashLight", programState->flashLight);

            // directional light setup
            lights_shader.setVec3("dirLight.direction", 1.0f, -0.5f, 0.0f);
            lights_shader.setVec3("dirLight.ambient", 0.01f, 0.01f, 0.01f);
            lights_shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
            lights_shader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

            // point light setup
            lights_shader.setVec3("pointLight.position", programState->pointLightSource);
            lights_shader.setVec3("pointLight.ambient", pointLight.ambient);
            lights_shader.setVec3("pointLight.diffuse", pointLight.diffuse);
            lights_shader.setVec3("pointLight.specular", pointLight.specular);
            lights_shader.setFloat("pointLight.constant", pointLight.constant);
            lights_shader.setFloat("pointLight.linear", pointLight.linear);
            lights_shader.setFloat("pointLight.quadratic", pointLight.quadratic);

            // spotlight setup
            lights_shader.setVec3("spotLight.position", camera.Position);
            lights_shader.setVec3("spotLight.direction", camera.Front);
            lights_shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            lights_shader.setVec3("spotLight.diffuse", 0.7f, 0.7f, 0.7f);
            lights_shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            lights_shader.setFloat("spotLight.constant", 1.0f);
            lights_shader.setFloat("spotLight.linear", 0.05);
            lights_shader.setFloat("spotLight.quadratic", 0.012);
            lights_shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.5f)));
            lights_shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(13.0f)));
        }

        // don't forget to enable shader_ before setting uniforms
        pointLight.position = glm::vec3(4.0 * cos(currentFrame), 4.0f, 4.0 * sin(currentFrame));

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                                (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = programState->camera.GetViewMatrix();

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, programState->backpackPosition); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(programState->backpackScale));    // it's a bit too big for our scene, so scale it down
        lights_shader.use();
        lights_shader.setMat4("model", model);
        ourModel.Draw(lights_shader);

        auto skybox_view = glm::mat4(glm::mat3(programState->camera.GetViewMatrix()));
        skybox.render(glm::mat4(1.0f), skybox_view, projection);

        // world transformation
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.51f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(25.0f));
        floor.render(model, view, projection);

        if (programState->ImGuiEnabled) {
            imGui.draw(programState.get());
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        window->swapBuffers();
        window->pollEvents();
    }

    programState->SaveToFile("resources/program_state.txt");
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
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
    auto& firstMouse = programState->firstMouse;
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

    if (programState->CameraMouseMovementUpdateEnabled)
        programState->camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    programState->camera.ProcessMouseScroll(yoffset);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        programState->ImGuiEnabled = !programState->ImGuiEnabled;
        if (programState->ImGuiEnabled) {
            programState->CameraMouseMovementUpdateEnabled = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
}

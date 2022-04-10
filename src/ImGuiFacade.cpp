//
// Created by smiga287 on 3/28/22.
//

#include "ImGuiFacade.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <program_state.h>

ImGuiFacade::ImGuiFacade(GlfwWindow* window) {
    // Init Imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;

    ImGui_ImplGlfw_InitForOpenGL(window->get_raw_window(), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void ImGuiFacade::draw(ProgramState* programState) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    {
        ImGui::Begin("Weird dream");
        ImGui::ColorEdit3("Background color", (float *) &programState->clearColor);
        ImGui::DragFloat3("Building position", (float*)&programState->building_position);
        ImGui::DragFloat("Building scale", &programState->building_scale, 0.05, 0.1, 8.0);

        ImGui::Checkbox("Flashlight", &programState->is_flashlight_enabled);
        ImGui::Checkbox("Blinn", &programState->is_blinn_enabled);
        ImGui::Checkbox("Bloom", &programState->is_bloom_enabled);
        ImGui::DragFloat("Exposure", &programState->exposure, 0.05, 0.0, 1.0);

        ImGui::DragFloat("pointLight.constant", &programState->pointLight.constant, 0.05, 0.0, 1.0);
        ImGui::DragFloat("pointLight.linear", &programState->pointLight.linear, 0.05, 0.0, 1.0);
        ImGui::DragFloat("pointLight.quadratic", &programState->pointLight.quadratic, 0.05, 0.0, 1.0);
        ImGui::End();
    }

    {
        ImGui::Begin("Camera info");
        const Camera& c = programState->camera;
        ImGui::Text("Camera position: (%f, %f, %f)", c.Position.x, c.Position.y, c.Position.z);
        ImGui::Text("(Yaw, Pitch): (%f, %f)", c.Yaw, c.Pitch);
        ImGui::Text("Camera front: (%f, %f, %f)", c.Front.x, c.Front.y, c.Front.z);
        ImGui::Checkbox("Camera mouse update", &programState->is_camera_mouse_movement_enabled);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

ImGuiFacade::~ImGuiFacade() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
//
// Created by smiga287 on 3/28/22.
//

#ifndef FUTURAMA_IMGUIFACADE_H
#define FUTURAMA_IMGUIFACADE_H

#include <GlfwWindow.h>

struct ProgramState;

class ImGuiFacade {
public:
    explicit ImGuiFacade(GlfwWindow* window);
    void draw(ProgramState* state);
    // Given that we have a destructor, we should follow the rule of 5
    ~ImGuiFacade();
    // For now, we'll delete the rest of methods required by the rule of 5
    ImGuiFacade(const ImGuiFacade& imGui) = delete;
    ImGuiFacade(ImGuiFacade&& imGui) noexcept = delete;
    ImGuiFacade& operator=(const ImGuiFacade& imGui) = delete;
    ImGuiFacade& operator=(ImGuiFacade&& imGui) noexcept = delete;
};

#endif //FUTURAMA_IMGUIFACADE_H

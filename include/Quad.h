//
// Created by smiga287 on 4/9/22.
//

#ifndef FUTURAMA_QUAD_H
#define FUTURAMA_QUAD_H

#include <program_state.h>
#include <array>

class Quad {
public:
    Quad();
    void render() const;
private:
    uint VAO_ = 0;
    uint VBO_;
    constexpr static std::array vertices_ = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
};

#endif //FUTURAMA_QUAD_H

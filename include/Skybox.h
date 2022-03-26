//
// Created by smiga287 on 3/26/22.
//

#ifndef PROJECT_BASE_SKYBOX_H
#define PROJECT_BASE_SKYBOX_H

#include <vector>
#include <string>
#include <array>
#include <span>
#include <glm/glm.hpp>

#include <learnopengl/shader.h>

class Skybox {
public:
    explicit Skybox(const Shader& shader);

    void loadTextures();
    void render(const glm::mat4& view, const glm::mat4& projection);

    // Given that we have a destructor, we should follow the rule of 5
    ~Skybox();
    // For now, we'll delete the rest of methods required by the rule of 5
    Skybox(const Skybox& skybox) = delete;
    Skybox(Skybox&& skybox) noexcept = delete;
    Skybox& operator=(const Skybox& skybox) = delete;
    Skybox& operator=(Skybox&& skybox) noexcept = delete;
private:
    void initOpenGL();
    void deinitOpenGL();
    uint loadCubemap(std::span<std::string> faces);

    Shader shader_;
    uint cubemap_texture_;

    uint skybox_VAO_;
    uint skybox_VBO_;

    // skybox coordinates
    constexpr static std::array skybox_vertices_ = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };
};


#endif //PROJECT_BASE_SKYBOX_H

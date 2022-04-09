//
// Created by smiga287 on 3/28/22.
//

#ifndef FUTURAMA_FLOOR_H
#define FUTURAMA_FLOOR_H

#include <vector>
#include <string>
#include <array>
#include <span>
#include <glm/glm.hpp>

#include <learnopengl/shader.h>

class Floor {
public:
    explicit Floor(const Shader& shader);

    void loadTextures();
    void render(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) const;

    // Given that we have a destructor, we should follow the rule of 5
    ~Floor();
    // For now, we'll delete the rest of methods required by the rule of 5
    Floor(const Floor& floor) = delete;
    Floor(Floor&& floor) noexcept = delete;
    Floor& operator=(const Floor& floor) = delete;
    Floor& operator=(Floor&& floor) noexcept = delete;
private:
    void initOpenGL();
    void deinitOpenGL();
    uint loadTexture(const std::string& path);

    Shader shader_;

    uint floor_VAO_;
    uint floor_VBO_;
    uint floor_EBO_;

    uint floor_specular_map_;
    uint floor_diffuse_map_;

    // floor coordinates
    constexpr static std::array floor_vertices_ = {
            // positions          // normals          // texture coords
            0.5f,   0.5f,  0.0f,  0.0f, 0.0f, -1.0f,  1.0f,  1.0f,  // top right
            0.5f,  -0.5f,  0.0f,  0.0f, 0.0f, -1.0f,  1.0f,  0.0f,  // bottom right
            -0.5f, -0.5f,  0.0f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,  // bottom left
            -0.5f,  0.5f,  0.0f,  0.0f, 0.0f, -1.0f,  0.0f,  1.0f   // top left
    };

    // floor vertices for use in EBO
    constexpr static std::array floor_indices_ = {
            0u, 1u, 3u,  // first triangle
            1u, 2u, 3u   // second triangle
    };
};


#endif //FUTURAMA_FLOOR_H

//
// Created by smiga287 on 3/28/22.
//

#include "Floor.h"
#include <learnopengl/filesystem.h>
#include <stb_image.h>
#include <glm/glm.hpp>

Floor::Floor(const Shader &shader) : shader_(shader) {
    initOpenGL();
}

void Floor::initOpenGL() {
    // Floor setup
    glGenVertexArrays(1, &floor_VAO_);
    glGenBuffers(1, &floor_VBO_);
    glGenBuffers(1, &floor_EBO_);

    glBindVertexArray(floor_VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, floor_VBO_);
    glBufferData(GL_ARRAY_BUFFER, floor_vertices_.size() * sizeof(float), floor_vertices_.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, floor_indices_.size() * sizeof(uint), floor_indices_.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

Floor::~Floor() {
    deinitOpenGL();
}

void Floor::deinitOpenGL() {
    glDeleteVertexArrays(1, &floor_VAO_);
    glDeleteBuffers(1, &floor_VBO_);
    glDeleteBuffers(1, &floor_EBO_);
}

void Floor::loadTextures() {
    floor_specular_map_ = loadTexture(FileSystem::getPath("resources/textures/floor_diffuse_map.jpg"));
    floor_diffuse_map_ = loadTexture(FileSystem::getPath("resources/textures/floor_specular_map.jpg"));

    shader_.use();
    shader_.setInt("material.diffuse", 0);
    shader_.setInt("material.specular", 1);
}

uint Floor::loadTexture(const std::string& path) {
    uint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (!data) {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    GLenum format = [nrComponents]() {
        switch (nrComponents) {
            case 1:  return GL_RED;
            case 3:  return GL_RGB;
            case 4:  return GL_RGBA;
            default: return GL_RED;
        }
    }();

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return textureID;
}

void Floor::render(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) const {
    shader_.use();
    shader_.setMat4("model", model);
    shader_.setMat4("view", view);
    shader_.setMat4("projection", projection);

    // floor setup
    // light properties
//    shader_.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    // material properties
    shader_.setFloat("material.shininess", 256.0f);

    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floor_diffuse_map_);

    // bind specular map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, floor_specular_map_);

    // render floor
    glBindVertexArray(floor_VAO_);
    glEnable(GL_CULL_FACE); // floor won't be visible if looked from bellow
    glCullFace(GL_BACK);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glDisable(GL_CULL_FACE);
}

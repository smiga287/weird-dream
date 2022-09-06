//
// Created by smiga287 on 3/26/22.
//
#include "Skybox.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <span>
#include <stb_image.h>

#include <iostream>
#include <learnopengl/filesystem.h>

void Skybox::loadTextures() {
  const std::string SKYBOX_PATH =
      FileSystem::getPath("resources/textures/skybox_dark_space/");
  const std::string EXT = false ? ".jpg" : ".png";
  std::array skyboxSides = {SKYBOX_PATH + "rt" + EXT, SKYBOX_PATH + "lf" + EXT,
                            SKYBOX_PATH + "up" + EXT, SKYBOX_PATH + "dn" + EXT,
                            SKYBOX_PATH + "bk" + EXT, SKYBOX_PATH + "ft" + EXT};
  cubemap_texture_ = loadCubemap(skyboxSides);
  shader_.use();
  shader_.setInt("skybox", 0);
}

// loads a cubemap texture from 6 individual texture faces in order:
// +X (right), -X (left), +Y (top), -Y (bottom), +Z (front), -Z (back)
uint Skybox::loadCubemap(std::span<std::string> faces) {
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int width, height, nrChannels;
  for (uint i = 0; i < faces.size(); i++) {
    auto data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
      std::cerr << "Cubemap texture failed to load at path: " << faces[i]
                << std::endl;
    }
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return textureID;
}

void Skybox::render(const glm::mat4 &model, const glm::mat4 &view,
                    const glm::mat4 &projection) const {
  // skybox shader setup
  glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when
                          // values are equal to depth buffer's content
  shader_.use();
  shader_.setMat4("view", view);
  shader_.setMat4("projection", projection);

  // render skybox cube
  glBindVertexArray(skybox_VAO_);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture_);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS); // set depth function back to default
}

void Skybox::initOpenGL() {
  // Skybox setup
  glGenVertexArrays(1, &skybox_VAO_);
  glGenBuffers(1, &skybox_VBO_);

  glBindVertexArray(skybox_VAO_);
  glBindBuffer(GL_ARRAY_BUFFER, skybox_VBO_);
  glBufferData(GL_ARRAY_BUFFER, skybox_vertices_.size() * sizeof(float),
               skybox_vertices_.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
}

void Skybox::deinitOpenGL() {
  glDeleteVertexArrays(1, &skybox_VAO_);
  glDeleteBuffers(1, &skybox_VBO_);
}

Skybox::Skybox(const Shader &shader) : shader_(shader) { initOpenGL(); }

Skybox::~Skybox() { deinitOpenGL(); }

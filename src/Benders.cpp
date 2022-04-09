#include <Benders.h>
#include <learnopengl/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

Benders::Benders(const Model& bender_model, const Shader& shader, int size) : bender_model_(bender_model), SIZE(size), models_(generate_random_models(size)), shader_(shader) {
    initOpenGL();
}

Benders::~Benders() {
    deinitOpenGL();
}

std::vector<glm::mat4> Benders::generate_random_models(int n) {
    std::vector<glm::mat4> models(n, glm::mat4(1.0f));
    for (auto& model : models) {
        // translation
        model = glm::translate(model, glm::vec3(random_.get_random() % 100 - 50,
                                         6 + random_.get_random() % 10,
                                         random_.get_random() % 100 - 50));

        // scale
        float scale = (random_.get_random() % 20) / 100.0f + 0.1f;
        model = glm::scale(model, glm::vec3(scale));

        // rotation
        float angle = (random_.get_random() % 60);
        model = glm::rotate(model, angle, glm::vec3(0.3f, 0.5f, 0.9f));
    }
    return models;
}

void Benders::initOpenGL() {
    uint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, SIZE * sizeof(glm::mat4), &models_[0], GL_STATIC_DRAW);

    for (const auto& mesh : bender_model_.meshes) {
        glBindVertexArray(mesh.VAO());
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
}

void Benders::deinitOpenGL() {
    // TODO: check this out
//    for (uint i = 0; i < SIZE; i++){
//        glDeleteVertexArrays(1, &(bender_model_.meshes[i].VAO()));
//    }
}

void Benders::render() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bender_model_.textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
    for (const auto& mesh : bender_model_.meshes) {
        glBindVertexArray(mesh.VAO());
        glDrawElementsInstanced(GL_TRIANGLES, mesh.indices().size(), GL_UNSIGNED_INT, nullptr, SIZE);
        glBindVertexArray(0);
    }
}
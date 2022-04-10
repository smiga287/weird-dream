#ifndef FUTURAMA_BENDERS_H
#define FUTURAMA_BENDERS_H

#include <vector>
#include <glm/glm.hpp>
#include <Random.h>
#include <learnopengl/model.h>

class Shader;

class Benders {
public:
    explicit Benders(const Model& bender_model, const Shader& shader, int size);

    void render() const;

    // Given that we have a destructor, we should follow the rule of 5
    ~Benders();
    // For now, we'll delete the rest of methods required by the rule of 5
    Benders(const Benders& benders) = delete;
    Benders(Benders&& benders) noexcept = delete;
    Benders& operator=(const Benders& benders) = delete;
    Benders& operator=(Benders&& benders) noexcept = delete;
private:
    void initOpenGL();
    void deinitOpenGL();

    std::vector<glm::mat4> generate_random_models(int n);

    Model bender_model_;
    Random random_;

    const int SIZE;
    std::vector<glm::mat4> models_;
    const Shader& shader_;
};

#endif //FUTURAMA_BENDERS_H

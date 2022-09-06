#ifndef FUTURAMA_INSTANCES_H
#define FUTURAMA_INSTANCES_H

#include <Random.h>
#include <glm/glm.hpp>
#include <learnopengl/model.h>
#include <vector>

class Shader;

class Instances {
public:
  explicit Instances(const Model &instance_model, const Shader &shader,
                     int size);

  void render() const;

  // Given that we have a destructor, we should follow the rule of 5
  ~Instances();
  // For now, we'll delete the rest of methods required by the rule of 5
  Instances(const Instances &benders) = delete;
  Instances(Instances &&benders) noexcept = delete;
  Instances &operator=(const Instances &benders) = delete;
  Instances &operator=(Instances &&benders) noexcept = delete;

private:
  void initOpenGL();
  void deinitOpenGL();

  std::vector<glm::mat4> generate_random_models(int n);

  Model bender_model_;
  Random random_;

  const int SIZE;
  std::vector<glm::mat4> models_;
  const Shader &shader_;
};

#endif // FUTURAMA_INSTANCES_H

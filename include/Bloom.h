#ifndef FUTURAMA_BLOOM_H
#define FUTURAMA_BLOOM_H

#include <Quad.h>
#include <array>
#include <learnopengl/shader.h>
#include <program_state.h>

class Bloom {
public:
  Bloom(const Shader &lights_shader, const Shader &lights_box_shader,
        const Shader &lights_blur_shader, const Shader &lights_merger_shader);
  void setRenderScene() const;
  void computeGaussianBlur();
  void renderToQuadWithHDR(const ProgramState &state);

private:
  void initOpenGL();
  void deinitOpenGL();

  const Shader &lights_shader_;
  const Shader &lights_box_shader_;
  const Shader &lights_blur_shader_;
  const Shader &lights_merger_shader_;

  Quad quad_;
  bool horizontal_ = true;

  uint hdrFBO_;
  std::array<uint, 2> colorBuffers_;
  uint rboDepth_;
  std::array<uint, 2> attachments_ = {GL_COLOR_ATTACHMENT0,
                                      GL_COLOR_ATTACHMENT1};

  std::array<uint, 2> pingpongFBO_;
  std::array<uint, 2> pingpongColorbuffers_;
};

#endif // FUTURAMA_BLOOM_H

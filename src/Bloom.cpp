#include <Bloom.h>
#include <glad/glad.h>
#include <iostream>

void Bloom::initOpenGL() {
  // configure (floating point) framebuffers
  glGenFramebuffers(1, &hdrFBO_);
  glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO_);
  // create 2 floating point color buffers (1 for normal rendering, other for
  // brightness threshold values)
  glGenTextures(2, colorBuffers_.data());
  for (uint i = 0; i < 2; i++) {
    glBindTexture(GL_TEXTURE_2D, colorBuffers_[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0,
                 GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would
                           // otherwise sample repeated texture values!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                           GL_TEXTURE_2D, colorBuffers_[i], 0);
  }
  // create and attach depth buffer (renderbuffer)
  glGenRenderbuffers(1, &rboDepth_);
  glBindRenderbuffer(GL_RENDERBUFFER, rboDepth_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH,
                        SCR_HEIGHT);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, rboDepth_);
  // tell OpenGL which color attachments we'll use (of this framebuffer) for
  // rendering
  glDrawBuffers(2, attachments_.data());
  // finally check if framebuffer is complete
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "Framebuffer not complete!" << std::endl;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // ping-pong-framebuffer for blurring
  glGenFramebuffers(2, pingpongFBO_.data());
  glGenTextures(2, pingpongColorbuffers_.data());
  for (uint i = 0; i < 2; i++) {
    glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO_[i]);
    glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers_[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0,
                 GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would
                           // otherwise sample repeated texture values!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           pingpongColorbuffers_[i], 0);
    // also check if framebuffers are complete (no need for depth buffer)
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      std::cerr << "Framebuffer not complete!" << std::endl;
    }
  }
}

void Bloom::deinitOpenGL() {}

void Bloom::setRenderScene() const {
  glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO_);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Bloom::computeGaussianBlur() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  bool first_iteration = true;
  unsigned int amount = 10;
  lights_blur_shader_.use();
  for (uint i = 0; i < amount; i++) {
    glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO_[horizontal_]);
    lights_blur_shader_.setInt("horizontal", horizontal_);
    glBindTexture(
        GL_TEXTURE_2D,
        first_iteration
            ? colorBuffers_[1]
            : pingpongColorbuffers_[!horizontal_]); // bind texture of other
                                                    // framebuffer (or scene if
                                                    // first iteration)
    quad_.render();
    horizontal_ = !horizontal_;
    if (first_iteration)
      first_iteration = false;
  }
}

void Bloom::renderToQuadWithHDR(const ProgramState &state) {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  lights_merger_shader_.use();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, colorBuffers_[0]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers_[!horizontal_]);
  lights_merger_shader_.setInt("bloom", state.is_bloom_enabled);
  lights_merger_shader_.setFloat("exposure", state.exposure);
  quad_.render();
}

Bloom::Bloom(const Shader &lights_shader, const Shader &lights_box_shader,
             const Shader &lights_blur_shader,
             const Shader &lights_merger_shader)
    : lights_shader_(lights_shader), lights_box_shader_(lights_box_shader),
      lights_blur_shader_(lights_blur_shader),
      lights_merger_shader_(lights_merger_shader) {
  initOpenGL();
}

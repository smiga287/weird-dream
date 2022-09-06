#include <Cube.h>

Cube::Cube() {
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
  // fill buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(float),
               vertices_.data(), GL_STATIC_DRAW);
  // link vertex attributes
  glBindVertexArray(VAO_);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Cube::render() const {
  glBindVertexArray(VAO_);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}

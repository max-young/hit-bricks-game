#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Texture.h"

// sprite渲染器
class SpriteRenderer
{
public:
  SpriteRenderer(Shader &shader);

  void DrawSprite(const Texture2D &texture, glm::vec2 position,
                  glm::vec2 size = glm::vec2(10, 10), float rotate = 0.0f,
                  glm::vec3 color = glm::vec3(1.0f));

private:
  Shader shader;
  GLuint quadVAO;  // vertex array object

  void initRenderData();
};

#endif

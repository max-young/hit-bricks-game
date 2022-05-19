#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"

class GameObject
{
public:
  glm::vec2 position;
  glm::vec2 size;
  glm::vec3 Color;
  GLfloat Rotation;
  GLboolean isSolid;
  Texture2D Texture;
  glm::vec2 velocity;
  GLboolean destroyed = false;

  GameObject();
  GameObject(glm::vec2 position, glm::vec2 size, Texture2D texture, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

  virtual void Draw(SpriteRenderer &renderer);
};

#endif
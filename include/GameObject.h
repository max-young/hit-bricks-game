#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"

class GameObject
{
public:
  glm::vec2 Position;
  glm::vec2 Size;
  glm::vec3 Color;
  GLfloat Rotation;
  GLboolean IsSolid;
  Texture2D Texture;

  GameObject();
  GameObject(glm::vec2 position, glm::vec2 size, Texture2D texture, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));

  virtual void Draw(SpriteRenderer &renderer);
};

#endif
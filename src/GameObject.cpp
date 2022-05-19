#include "GameObject.h"

GameObject::GameObject()
    : position(0.0f, 0.0f), size(1.0f, 1.0f), Rotation(0.0f), Color(1.0f), Texture(), isSolid(false), velocity(0.0f) {}

GameObject::GameObject(glm::vec2 position, glm::vec2 size, Texture2D texture, glm::vec3 color, glm::vec2 velocity)
  : position(position), size(size), Texture(texture), Color(color), Rotation(0.0f), isSolid(false), velocity(velocity) {}

void GameObject::Draw(SpriteRenderer &renderer)
{
  renderer.DrawSprite(this->Texture, this->position, this->size, this->Rotation, this->Color);
}
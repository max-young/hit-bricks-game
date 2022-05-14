#include "GameObject.h"

GameObject::GameObject()
    : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Rotation(0.0f), Color(1.0f), Texture(), IsSolid(false) {}

GameObject::GameObject(glm::vec2 position, glm::vec2 size, Texture2D texture, glm::vec3 color)
  : Position(position), Size(size), Texture(texture), Color(color), Rotation(0.0f), IsSolid(false) {}

void GameObject::Draw(SpriteRenderer &renderer)
{
  renderer.DrawSprite(this->Texture, this->Position, this->Size, this->Rotation, this->Color);
}
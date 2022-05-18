#include "BallObject.h"

BallObject::BallObject():
  GameObject(), radius(12.5f), stuck(false) {}

BallObject::BallObject(glm::vec2 pos, GLfloat radius, Texture2D texture, glm::vec2 velocity):
  GameObject(pos, glm::vec2(radius * 2, radius * 2), texture, glm::vec3(1.0f), velocity), radius(radius), stuck(true) { }

glm::vec2 BallObject::move(GLfloat dt, GLuint windowWidth)
{
  if(!this->stuck)
  {
    this->position += this->velocity * dt;
    if(this->position.x <= 0.0f || this->position.x + this->size.x >= windowWidth)
      this->velocity.x = -this->velocity.x;
    if (this->position.y <= 0.0f)
      this->velocity.y = -this->velocity.y;
  }
  return this->position;
}
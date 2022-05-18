#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include "GameObject.h"

class BallObject : public GameObject
{
public:
  GLfloat radius;
  GLboolean stuck;

  BallObject();
  BallObject(glm::vec2 pos, GLfloat radius, Texture2D texture, glm::vec2 velocity);

  glm::vec2 move(GLfloat dt, GLuint windowWidth);
  void reset(glm::vec2 position, glm::vec2 velocity);
};

#endif
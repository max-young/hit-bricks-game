#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"


using std::vector;

struct Particle
{
  glm::vec2 position;
  glm::vec2 velocity;
  glm::vec4 color;
  GLfloat life;

  Particle() = default;
};

class ParticleGenerator
{
public:
  ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);
  void update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
  void draw();

private:
  vector<Particle> particles;
  GLuint amount;
  Shader shader;
  Texture2D texture;
  GLuint VAO;
  void init();
  GLuint firstUnusedParticle();
  void respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif
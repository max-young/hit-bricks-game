#include "Particle.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, GLuint amount)
    : shader(shader), texture(texture), amount(amount)
{
  this->init();
}

void ParticleGenerator::init()
{
  GLuint VBO;
  GLfloat particleQuad[] = {
      0.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f,

      0.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 1.0f, 1.0f, 1.0f,
      1.0f, 0.0f, 1.0f, 0.0f};
  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(this->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(particleQuad), particleQuad, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *)0);
  glBindVertexArray(0);

  for (GLuint i = 0; i < this->amount; ++i)
  {
    this->particles.push_back(Particle());
  }
}

void ParticleGenerator::update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset)
{
  for (GLuint i = 0; i < newParticles; ++i)
  {
    int unusedParticle = this->firstUnusedParticle();
    this->respawnParticle(this->particles[unusedParticle], object, offset);
  }
  for (GLuint i = 0; i < this->amount; ++i)
  {
    Particle &p = this->particles[i];
    p.life -= dt;
    if (p.life > 0.0f)
    {
      p.position -= p.velocity * dt;
      p.color.a -= dt * 5.5f;
    }
  }
}

GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle()
{
  for (GLuint i = lastUsedParticle; i < this->amount; ++i)
  {
    if (this->particles[i].life <= 0.0f)
    {
      lastUsedParticle = i;
      return i;
    }
  }
  for (GLuint i = 0; i < lastUsedParticle; ++i)
  {
    if (this->particles[i].life <= 0.0f)
    {
      lastUsedParticle = i;
      return i;
    }
  }
  lastUsedParticle = 0;
  return 0;
}

void ParticleGenerator::respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset)
{
  GLfloat random = (rand() % 100 - 50) / 10.0f;
  GLfloat rColor = (rand() % 100 + 50)/ 100.0f;
  particle.position = object.position + random + offset;
  particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
  particle.life = 0.5f;
  particle.velocity = object.velocity * 0.1f;
}

void ParticleGenerator::draw()
{
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  this->shader.use();
  for (Particle p : this->particles)
  {
    if (p.life > 0.0f)
    {
      this->shader.setVector4f("color", p.color);
      this->shader.setVector2f("offset", p.position);
      this->texture.bind();
      glBindVertexArray(this->VAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
    }
  }
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
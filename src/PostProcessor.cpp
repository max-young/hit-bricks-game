#include "PostProcessor.h"

PostProcessor::PostProcessor(Shader shader, GLuint width, GLuint height)
    : shader(shader), width(width), height(height), texture(), shake(false)
{
  glGenFramebuffers(1, &this->FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);

  this->texture.generate(width, height, NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture.ID, 0);

  glGenRenderbuffers(1, &this->RBO);
  glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->RBO);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::POSTPROCESSOR: Failed to initialize framebuffer" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  this->initRenderData();
  this->shader.setInteger("scene", 0);
}

void PostProcessor::beginRender()
{
  glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::endRender()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::initRenderData()
{
  unsigned int VBO;
  float vertices[] = {
      -1.0f, -1.0f, 0.0f, 0.0f,
      1.0f, 1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f, 0.0f, 1.0f,

      -1.0f, -1.0f, 0.0f, 0.0f,
      1.0f, -1.0f, 1.0f, 0.0f,
      1.0f, 1.0f, 1.0f, 1.0f};
  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(this->VAO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void PostProcessor::render(GLfloat time)
{
  this->shader.use();
  this->shader.setFloat("time", time);
  this->shader.setInteger("shake", this->shake);

  this->texture.bind();
  glBindVertexArray(this->VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}
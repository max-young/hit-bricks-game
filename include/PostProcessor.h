#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H

#include <iostream>
#include <glad/glad.h>

#include "Shader.h"
#include "Texture.h"

using std::cout; using std::endl;

class PostProcessor
{
public:
  Shader shader;
  Texture2D texture;
  GLuint width, height;
  GLboolean shake;

  PostProcessor(Shader shader, GLuint width, GLuint height);

  void beginRender();
  void endRender();
  void render(GLfloat time);

private:
  GLuint FBO;
  GLuint RBO;
  GLuint VAO;
  void initRenderData();
};

#endif
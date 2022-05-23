#include "Shader.h"

#include <iostream>

using std::cout; using std::endl;

Shader &Shader::use()
{
  glUseProgram(ID);
  return *this;
}

void Shader::setInteger(const string &name, int value)
{
  glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setVector2f(const string &name, const glm::vec2 &value)
{
  glUniform2f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y);
}

void Shader::setVector3f(const string &name, const glm::vec3 &value)
{
  glUniform3f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::setVector4f(const string &name, const glm::vec4 &value)
{
  glUniform4f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setMatrix4(const string &name, const glm::mat4 &matrix)
{
  glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::Compile(const string vertexSource, const string fragmentSource, const string geometrySource)
{
  unsigned int sVertex, sFragment, gShader;
  // vertex shader
  sVertex = glCreateShader(GL_VERTEX_SHADER);
  const char* vertexCode = vertexSource.c_str();
  glShaderSource(sVertex, 1, &vertexCode, NULL);
  glCompileShader(sVertex);
  checkCompileErrors(sVertex, "VERTEX");
  // fragment shader
  sFragment = glCreateShader(GL_FRAGMENT_SHADER);
  const char* fragmentCode = fragmentSource.c_str();
  glShaderSource(sFragment, 1, &fragmentCode, NULL);
  glCompileShader(sFragment);
  checkCompileErrors(sFragment, "FRAGMENT");
  // geometry shader
  if (geometrySource != "")
  {
    gShader = glCreateShader(GL_GEOMETRY_SHADER);
    const char* geometryCode = geometrySource.c_str();
    glShaderSource(gShader, 1, &geometryCode, NULL);
    glCompileShader(gShader);
    checkCompileErrors(gShader, "GEOMETRY");
  }
  // 创建program
  this->ID = glCreateProgram();
  glAttachShader(this->ID, sVertex);
  glAttachShader(this->ID, sFragment);
  if (geometrySource != "")
    glAttachShader(this->ID, gShader);
  glLinkProgram(this->ID);
  checkCompileErrors(this->ID, "PROGRAM");
  // program link shader之后, shader不需要了, 删除
  glDeleteShader(sVertex);
  glDeleteShader(sFragment);
  if (geometrySource != "")
    glDeleteShader(gShader);
}

void Shader::checkCompileErrors(unsigned int object, string type)
{
  int success;
  char infoLog[1024];
  if (type != "PROGRAM")
  {
    glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(object, 1024, NULL, infoLog);
      cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
        << infoLog << "\n -- --------------------------------------------------- -- "
        << endl;
    }
  }
  else
  {
    glGetProgramiv(object, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(object, 1024, NULL, infoLog);
      cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
        << infoLog << "\n -- --------------------------------------------------- -- "
        << endl;
    }
  }
}
#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

using std::string;

class Shader
{
public:
  // shader program的ID, compile时会创建
  unsigned int ID;

  Shader(){};

  Shader &use();

  // 编译创建shader
  void Compile(const string vertexSource, const string fragmentSource, const string geometrySource = "");

  void SetInteger(const string &name, int value);
  void SetVector3f(const string &name, const glm::vec3 &value);
  void SetMatrix4(const string &name, const glm::mat4 &matrix);

private:
  // 检查shader编译错误
  void checkCompileErrors(unsigned int object, string type);
};

#endif
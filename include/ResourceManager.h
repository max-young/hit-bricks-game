#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <map>

#include <glad/glad.h>

#include "Shader.h"
#include "Texture.h"

using std::map;
using std::string;

class ResourceManager
{
public:
  // key是shader的名字, value是shader
  static map<string, Shader> Shaders;
  // key是texture的名字, value是Texture2D
  static map<string, Texture2D> Textures;

  // 根据shader文件创建shader, 并存储在Shaders中
  static Shader LoadShader(const string &vShaderFile, const string &fShaderFile, const string &gShaderFile, string name);
  // 根据shader name获取Shader
  static Shader GetShader(const string &name);

  // 根据texture文件创建texture, 并存储在Textures中
  static Texture2D LoadTexture(const string &file, bool alpha, string name);
  // 根据texture name获取Texture2D
  static Texture2D GetTexture(const string &name);

private:
  ResourceManager(){};
  static Shader loadShaderFromFile(const string &vShaderFile, const string &fShaderFile, const string &gShaderFile = "");
  static Texture2D loadTextureFromFile(const string &file, bool alpha);
};
#endif
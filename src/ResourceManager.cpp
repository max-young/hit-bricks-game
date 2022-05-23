#include "ResourceManager.h"
#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "stb_image.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::map;
using std::ostringstream;
using std::string;

map<string, Shader> ResourceManager::Shaders;
map<string, Texture2D> ResourceManager::Textures;

Shader ResourceManager::loadShader(const string &vShaderFile, const string &fShaderFile, const string &gShaderFile, string name)
{
  Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
  return Shaders[name];
}

Shader ResourceManager::getShader(const string &name)
{
  return Shaders[name];
}

Texture2D ResourceManager::loadTexture(const string &file, bool alpha, string name)
{
  Textures[name] = loadTextureFromFile(file, alpha);
  return Textures[name];
}

Texture2D ResourceManager::getTexture(const string &name)
{
  return Textures[name];
}

Shader ResourceManager::loadShaderFromFile(const string &vShaderFile, const string &fShaderFile, const string &gShaderFile)
{
  string vertexCode;
  string fragmentCode;
  string geometryCode;
  // 打开shader文件
  try
  {
    ifstream vertexShaderFile(vShaderFile);
    ifstream fragmentShaderFile(fShaderFile);
    ostringstream vShaderStream, fShaderStream;

    vShaderStream << vertexShaderFile.rdbuf();
    fShaderStream << fragmentShaderFile.rdbuf();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

    if (gShaderFile != "")
    {
      std::ifstream geometryShaderFile(gShaderFile);
      std::stringstream gShaderStream;
      gShaderStream << geometryShaderFile.rdbuf();
      geometryCode = gShaderStream.str();
    }
  }
  catch (std::exception e)
  {
    cout << "ERROR::SHADER: Failed to read shader files" << endl;
  }
  // 根据文件创建shader
  Shader shader;
  shader.Compile(vertexCode, fragmentCode, geometryCode);
  return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const string &file, bool alpha)
{
  Texture2D texture;
  if (alpha)
  {
    texture.Internal_Format = GL_RGBA;
    texture.Image_Format = GL_RGBA;
  }
  // 加载图片
  int width, height, nrChannels;
  unsigned char *data = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);
  // 创建纹理
  texture.Generate(width, height, data);
  // 释放文件
  stbi_image_free(data);
  return texture;
}
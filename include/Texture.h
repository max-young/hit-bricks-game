#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture2D
{
public:
  unsigned int ID;
  unsigned int Width, Height;
  unsigned int Internal_Format; // texture格式
  unsigned int Image_Format;    // texture加载的图像格式
  unsigned int Wrap_S;          // 纹理s轴的重复方式
  unsigned int Wrap_T;          // 纹理t轴的重复方式
  unsigned int Filter_Min;      // 纹理最小滤波方式
  unsigned int Filter_Max;      // 纹理最大滤波方式

  Texture2D();

  // 生成texture
  void Generate(unsigned int width, unsigned int height, unsigned char *data);
  void bind() const; // 绑定texture到当前render program
};

#endif
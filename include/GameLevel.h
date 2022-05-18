#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

class GameLevel
{
public:
  // 砖块
  vector<GameObject> bricks;
  GameLevel(){};
  // 从文件里加载级别数据
  void Load(const string &file, GLuint width, GLuint height);
  void Draw(SpriteRenderer &renderer);

private:
  // 根据文件添加砖块到bricks中
  void init(vector<vector<GLuint>> tileData, GLuint width, GLuint height);
};

#endif
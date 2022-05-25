#include "GameLevel.h"

void GameLevel::Load(const string &file, GLuint width, GLuint height)
{
  // 清空之前的数据
  this->bricks.clear();
  // 加载文件
  GLuint tileCode;
  string line;
  ifstream ifs(file);
  vector<vector<GLuint>> tileData;
  if (ifs)
  {
    while (getline(ifs, line))
    {
      istringstream sstream(line);
      vector<GLuint> row;
      while (sstream >> tileCode)
      {
        row.push_back(tileCode);
      }
      tileData.push_back(row);
    }
    if (tileData.size() > 0)
      this->init(tileData, width, height);
  }
}

void GameLevel::init(vector<vector<GLuint>> tileData, GLuint lvlWidth, GLuint lvlHeight)
{
  // 计算每个级别的大小
  GLuint height = tileData.size();
  GLuint width = tileData[0].size();
  GLfloat unitWidth = lvlWidth / static_cast<GLfloat>(width);
  GLfloat unitHeight = lvlHeight / static_cast<GLfloat>(height);
  // 添加砖块
  for (GLuint y = 0; y < height; ++y)
  {
    for (GLuint x = 0; x < width; ++x)
    {
      glm::vec2 pos(unitWidth * x, unitHeight * y);
      glm::vec2 size(unitWidth, unitHeight);
      if (tileData[y][x] == 1)
      {
        GameObject obj(pos, size, ResourceManager::getTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
        obj.isSolid = GL_TRUE;
        this->bricks.push_back(obj);
      }
      else if (tileData[y][x] > 1)
      {
        glm::vec3 color(1.0f, 1.0f, 1.0f);
        if (tileData[y][x] == 2)
          color = glm::vec3(253.0f, 0.0f, 0.0f) / 255.0f;
        else if (tileData[y][x] == 3)
          color = glm::vec3(72.0f, 183.0f, 51.0f) / 255.0f;
        else if (tileData[y][x] == 4)
          color = glm::vec3(177.0f, 120.0f, 0.0f) / 255.0f;
        else if (tileData[y][x] == 5)
          color = glm::vec3(255.0f, 255.0f, 255.0f) / 255.0f;
        this->bricks.push_back(GameObject(pos, size, ResourceManager::getTexture("block"), color));
      }
    }
  }
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
  for (auto &brick : this->bricks)
  {
    if (!brick.destroyed)
      brick.Draw(renderer);
  }
}

GLboolean GameLevel::isCompleted()
{
  for (auto &brick : this->bricks)
  {
    if (!brick.isSolid && !brick.destroyed)
      return GL_FALSE;
  }
  return GL_TRUE;
}
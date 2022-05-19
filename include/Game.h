#ifndef GAME_H
#define GAME_H

#include "GameLevel.h"
#include "BallObject.h"

using std::tuple;
using std::get;

enum class GameState
{
  ACTIVE,
  MENU,
  WIN
};

enum class Direction
{
  UP,
  RIGHT,
  DOWN,
  LEFT
};

using Collision = tuple<GLboolean, Direction, glm::vec2>;

GLboolean checkCollision(const GameObject &a, const GameObject &b);
Collision checkCollision(const BallObject &a, const GameObject &b);
Direction vectorDirection(glm::vec2 direction);


class Game
{
public:
  // 游戏状态, 不同的状态渲染和处理不同的对象
  GameState state;
  GLboolean keys[1024];
  GLuint width, height;
  vector<GameLevel> levels;
  GLuint level;

  // constructor/deconstructor
  Game(unsigned int width, unsigned int height);

  // initialize game state (load all shader/textures/levels)
  void init();
  void render();
  void processInput(GLfloat dt);
  void update(GLfloat dt);
  void doCollision();
};

#endif
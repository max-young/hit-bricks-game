#ifndef GAME_H
#define GAME_H

#include "GameLevel.h"

enum GameState
{
  GAME_ACTIVE,
  GAME_MENU,
  GAME_WIN
};

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
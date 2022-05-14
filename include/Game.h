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
  GameState State;
  GLuint Width, Height;
  vector<GameLevel> Levels;
  GLuint Level;

  // constructor/deconstructor
  Game(unsigned int width, unsigned int height);

  // initialize game state (load all shader/textures/levels)
  void Init();

  void Render();
};

#endif
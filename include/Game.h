#ifndef GAME_H
#define GAME_H

enum GameState {
  GAME_ACTIVE,
  GAME_MENU,
  GAME_WIN
};

class Game
{
  public:
    // 游戏状态, 不同的状态渲染和处理不同的对象
    GameState State;
    bool Keys[1024];
    unsigned int Width, Height;

    // constructor/deconstructor
    Game(unsigned int width, unsigned int height);
    ~Game();

    // initialize game state (load all shader/textures/levels)
    void Init();

    void PrecessInput(float dt);
    void Update(float dt);
    void Render();
};

#endif
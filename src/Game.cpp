#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "FileSystem.h"

// #include <memory>

using std::make_shared;
using std::shared_ptr;

shared_ptr<SpriteRenderer> Renderer;

// 初始化挡板的大小
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
// 初始化挡板的速度
const GLfloat PLAYER_VELOCITY(500.0f);

shared_ptr<GameObject> Player;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Width(width), Height(height)
{
}

void Game::Init()
{
  // 加载shader
  ResourceManager::LoadShader(FileSystem::getPath("shaders/sprite.vs"), FileSystem::getPath("shaders/sprite.fs"), "", "sprite");
  // 正摄投影矩阵. 因为是2D游戏, 摄像机相当于在窗口中央不动, object已经在摄像机坐标系下, 只需做投影变换到[-1, 1]*[-1, 1]的canonical坐标系下
  glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
  Shader spriteShader = ResourceManager::GetShader("sprite");
  spriteShader.use();
  spriteShader.SetInteger("image", 0);
  spriteShader.SetMatrix4("projection", projection);

  Renderer = make_shared<SpriteRenderer>(SpriteRenderer(spriteShader));

  ResourceManager::LoadTexture(FileSystem::getPath("textures/awesomeface.png"), GL_TRUE, "face");
  ResourceManager::LoadTexture(FileSystem::getPath("textures/background.jpg"), GL_FALSE, "background");
  ResourceManager::LoadTexture(FileSystem::getPath("textures/block.png"), GL_FALSE, "block");
  ResourceManager::LoadTexture(FileSystem::getPath("textures/block_solid.png"), GL_FALSE, "block_solid");
  ResourceManager::LoadTexture(FileSystem::getPath("textures/paddle.png"), GL_TRUE, "paddle");

  GameLevel one; one.Load(FileSystem::getPath("levels/one.lvl"), this->Width, this->Height * 0.5f);
  GameLevel two; two.Load(FileSystem::getPath("levels/two.lvl"), this->Width, this->Height * 0.5f);
  GameLevel three; three.Load(FileSystem::getPath("levels/three.lvl"), this->Width, this->Height * 0.5f);
  GameLevel four; four.Load(FileSystem::getPath("levels/four.lvl"), this->Width, this->Height * 0.5f);

  this->Levels.push_back(one);
  this->Levels.push_back(two);
  this->Levels.push_back(three);
  this->Levels.push_back(four);
  this->Level = 0;

  glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
  Player = make_shared<GameObject>(GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle")));
}

void Game::Render()
{
  Renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(200, 200), glm::vec2(100, 100), 0.0f, glm::vec3(1.0f));
  if (this->State == GAME_ACTIVE)
  {
    Renderer -> DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
  }
  this->Levels[this->Level].Draw(*Renderer);
  Player->Draw(*Renderer);
}

void Game::ProcessInput(GLfloat dt)
{
  if (this->State == GAME_ACTIVE)
  {
    GLfloat velocity = PLAYER_VELOCITY * dt;
    if (this->Keys[GLFW_KEY_A])
    {
      if(Player->Position.x >= 0)
      {
        Player->Position.x -= velocity;
      }
    }
    if (this->Keys[GLFW_KEY_D])
    {
      if(Player->Position.x <= this->Width - Player->Size.x)
      {
        Player->Position.x += velocity;
      }
    }
  }
}

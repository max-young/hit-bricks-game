#include <glm/gtc/matrix_transform.hpp>

#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "FileSystem.h"

#include <memory>

using std::make_shared;
using std::shared_ptr;

shared_ptr<SpriteRenderer> Renderer;

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
}

void Game::Render()
{
  Renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(200, 200), glm::vec2(100, 100), 0.0f, glm::vec3(1.0f));
}

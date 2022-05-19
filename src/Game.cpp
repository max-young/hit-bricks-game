#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "FileSystem.h"
#include "BallObject.h"

// #include <memory>

using std::make_shared;
using std::shared_ptr;

shared_ptr<SpriteRenderer> Renderer;

// 初始化挡板的大小
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
// 初始化挡板的速度
const GLfloat PLAYER_VELOCITY(500.0f);

shared_ptr<GameObject> player;

// 初始化球的速度
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// 球的半径
const GLfloat BALL_RADIUS = 12.5f;

shared_ptr<BallObject> ball;

Game::Game(unsigned int width, unsigned int height)
    : state(GameState::ACTIVE), width(width), height(height)
{
}

void Game::init()
{
  // 加载shader
  ResourceManager::LoadShader(FileSystem::getPath("shaders/sprite.vs"), FileSystem::getPath("shaders/sprite.fs"), "", "sprite");
  // 正摄投影矩阵. 因为是2D游戏, 摄像机相当于在窗口中央不动, object已经在摄像机坐标系下, 只需做投影变换到[-1, 1]*[-1, 1]的canonical坐标系下
  glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->width), static_cast<GLfloat>(this->height), 0.0f, -1.0f, 1.0f);
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

  GameLevel one;
  one.Load(FileSystem::getPath("levels/one.lvl"), this->width, this->height * 0.5f);
  GameLevel two;
  two.Load(FileSystem::getPath("levels/two.lvl"), this->width, this->height * 0.5f);
  GameLevel three;
  three.Load(FileSystem::getPath("levels/three.lvl"), this->width, this->height * 0.5f);
  GameLevel four;
  four.Load(FileSystem::getPath("levels/four.lvl"), this->width, this->height * 0.5f);

  this->levels.push_back(one);
  this->levels.push_back(two);
  this->levels.push_back(three);
  this->levels.push_back(four);
  this->level = 0;

  glm::vec2 playerPos = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f, this->height - PLAYER_SIZE.y);
  player = make_shared<GameObject>(GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle")));

  glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
  ball = make_shared<BallObject>(BallObject(ballPos, BALL_RADIUS, ResourceManager::GetTexture("face"), INITIAL_BALL_VELOCITY));
}

void Game::render()
{
  if (this->state == GameState::ACTIVE)
  {
    Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->width, this->height), 0.0f);
    this->levels[this->level].Draw(*Renderer);
    player->Draw(*Renderer);
    ball->Draw(*Renderer);
  }
}

void Game::processInput(GLfloat dt)
{
  if (this->state == GameState::ACTIVE)
  {
    GLfloat velocity = PLAYER_VELOCITY * dt;
    if (this->keys[GLFW_KEY_A])
    {
      if (player->position.x >= 0)
      {
        player->position.x -= velocity;
        if (ball->stuck)
        {
          ball->position.x -= velocity;
        }
      }
    }
    if (this->keys[GLFW_KEY_D])
    {
      if (player->position.x <= this->width - player->size.x)
      {
        player->position.x += velocity;
        if (ball->stuck)
        {
          ball->position.x += velocity;
        }
      }
    }
    if (this->keys[GLFW_KEY_SPACE])
      ball->stuck = false;
  }
}

void Game::doCollision()
{
  for (auto &box : this->levels[this->level].bricks)
  {
    if (!box.destroyed)
    {
      Collision collision = checkCollision(*ball, box);
      if (get<0>(collision))
      {
        if (!box.isSolid)
        {
          box.destroyed = true;
        }
        Direction direction = get<1>(collision);
        glm::vec2 diffVector = get<2>(collision);
        if (direction == Direction::UP || direction == Direction::DOWN)
        {
          ball->velocity.y *= -1;
          GLfloat penetration = ball->radius - abs(diffVector.y);
          if (direction == Direction::UP)
            ball->position.y -= penetration;
          else if (direction == Direction::DOWN)
            ball->position.y += penetration;
        }
        else if (direction == Direction::LEFT || direction == Direction::RIGHT)
        {
          ball->velocity.x *= -1;
          GLfloat penetration = ball->radius - abs(diffVector.x);
          if (direction == Direction::LEFT)
            ball->position.x += penetration;
          else if (direction == Direction::RIGHT)
            ball->position.x -= penetration;
        }
      }
    }
  }
}

void Game::update(GLfloat dt)
{
  ball->move(dt, this->width);
  this->doCollision();
}


GLboolean checkCollision(const GameObject &a, const GameObject &b)
{
  GLboolean collisionX = (a.position.x + a.size.x >= b.position.x) && (b.position.x + b.size.x >= a.position.x);
  GLboolean collisionY = (a.position.y + a.size.y >= b.position.y) && (b.position.y + b.size.y >= a.position.y);
  return collisionX && collisionY;
}

Collision checkCollision(const BallObject &a, const GameObject &b)
{
  // 圆的中心
  glm::vec2 center(a.position + a.radius);
  // AABB的半边长和中心
  glm::vec2 aabbHalfExtents(b.size.x / 2.0f, b.size.y / 2.0f);
  glm::vec2 aabbCenter(b.position.x + aabbHalfExtents.x, b.position.y + aabbHalfExtents.y);
  // 获取两个中心的差矢量
  glm::vec2 difference = center - aabbCenter;
  glm::vec2 clamped = glm::clamp(difference, -aabbHalfExtents, aabbHalfExtents);
  glm::vec2 closest = aabbCenter + clamped;
  difference = closest - center;
  if (glm::length(difference) < a.radius)
    return Collision{GL_TRUE, vectorDirection(difference), difference};
  else
    return Collision{GL_FALSE, Direction::UP, glm::vec2(0, 0)};
}

Direction vectorDirection(glm::vec2 direction)
{
  glm::vec2 normalDirection = normalize(direction);
  if (abs(normalDirection.x) < abs(normalDirection.y))
  {
    if (normalDirection.y > 0.0f)
      return Direction::UP;
    else
      return Direction::DOWN;
  }
  else
  {
    if (normalDirection.x > 0.0f)
      return Direction::RIGHT;
    else
      return Direction::LEFT;
  }
}
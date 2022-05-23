#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "FileSystem.h"
#include "BallObject.h"
#include "Particle.h"

using std::make_unique;
using std::unique_ptr;

unique_ptr<SpriteRenderer> Renderer;

// 初始化挡板的大小
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
// 初始化挡板的速度
const GLfloat PLAYER_VELOCITY(500.0f);

unique_ptr<GameObject> player;

// 初始化球的速度
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// 球的半径
const GLfloat BALL_RADIUS = 12.5f;

unique_ptr<BallObject> ball;

// 球的火焰
unique_ptr<ParticleGenerator> particles;

Game::Game(unsigned int width, unsigned int height)
    : state(GameState::ACTIVE), width(width), height(height)
{
}

void Game::init()
{
  // 加载shader
  ResourceManager::loadShader(FileSystem::getPath("shaders/sprite.vs"), FileSystem::getPath("shaders/sprite.fs"), "", "sprite");
  ResourceManager::loadShader(FileSystem::getPath("shaders/particle.vs"), FileSystem::getPath("shaders/particle.fs"), "", "particle");
  
  // 正摄投影矩阵. 因为是2D游戏, 摄像机相当于在窗口中央不动, object已经在摄像机坐标系下, 只需做投影变换到[-1, 1]*[-1, 1]的canonical坐标系下
  glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->width), static_cast<GLfloat>(this->height), 0.0f, -1.0f, 1.0f);
  Shader spriteShader = ResourceManager::getShader("sprite");
  spriteShader.use();
  spriteShader.setInteger("image", 0);
  spriteShader.setMatrix4("projection", projection);
  Shader particleShader = ResourceManager::getShader("particle");
  particleShader.use();
  particleShader.setMatrix4("projection", projection);

  Renderer = make_unique<SpriteRenderer>(SpriteRenderer(spriteShader));

  ResourceManager::loadTexture(FileSystem::getPath("textures/awesomeface.png"), GL_TRUE, "face");
  ResourceManager::loadTexture(FileSystem::getPath("textures/background.jpg"), GL_FALSE, "background");
  ResourceManager::loadTexture(FileSystem::getPath("textures/block.png"), GL_FALSE, "block");
  ResourceManager::loadTexture(FileSystem::getPath("textures/block_solid.png"), GL_FALSE, "block_solid");
  ResourceManager::loadTexture(FileSystem::getPath("textures/paddle.png"), GL_TRUE, "paddle");
  ResourceManager::loadTexture(FileSystem::getPath("textures/particle.png"), GL_TRUE, "particle");

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
  player = make_unique<GameObject>(GameObject(playerPos, PLAYER_SIZE, ResourceManager::getTexture("paddle")));

  glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
  ball = make_unique<BallObject>(BallObject(ballPos, BALL_RADIUS, ResourceManager::getTexture("face"), INITIAL_BALL_VELOCITY));

  particles = make_unique<ParticleGenerator>(ParticleGenerator(ResourceManager::getShader("particle"), ResourceManager::getTexture("particle"), 500));
}

void Game::render()
{
  if (this->state == GameState::ACTIVE)
  {
    Renderer->DrawSprite(ResourceManager::getTexture("background"), glm::vec2(0, 0), glm::vec2(this->width, this->height), 0.0f);
    this->levels[this->level].Draw(*Renderer);
    player->Draw(*Renderer);
    particles->draw();
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
  Collision result = checkCollision(*ball, *player);
  if (!ball->stuck && get<0>(result))
  {
    GLfloat centerBoard = player->position.x + player->size.x / 2;
    GLfloat distance = ball->position.x + ball->radius - centerBoard;
    GLfloat percentage = distance / (player->size.x/2);
    GLfloat strength = 2.0f;
    glm::vec2 oldVelocity = ball->velocity;
    ball->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
    ball->velocity.y = -1 * abs(ball->velocity.y);
    ball->velocity = glm::normalize(ball->velocity) * glm::length(oldVelocity);
  }
}

void Game::update(GLfloat dt)
{
  ball->move(dt, this->width);
  particles->update(dt, *ball, 2, glm::vec2(ball->radius / 2));
  this->doCollision();
  if (ball->position.y > this->height)
  {
    this->resetLevel();
    this->resetPlayer();
  }
  if (this->levels[this->level].isCompleted())
  {
    this->level++;
    this->resetPlayer();
  }
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

void Game::resetLevel()
{
  switch (this->level)
  {
    case 0:
      this->levels[this->level].Load(FileSystem::getPath("levels/one.lvl"), this->width, this->height * 0.5f);
      break;
    case 1:
      this->levels[this->level].Load(FileSystem::getPath("levels/two.lvl"), this->width, this->height * 0.5f);
      break;
    case 2:
      this->levels[this->level].Load(FileSystem::getPath("levels/three.lvl"), this->width, this->height * 0.5f);
      break;
    case 3:
      this->levels[this->level].Load(FileSystem::getPath("levels/four.lvl"), this->width, this->height * 0.5f);
      break;
  }
}

void Game::resetPlayer()
{
  glm::vec2 playerPos = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f, this->height - PLAYER_SIZE.y);
  player->position = playerPos;
  ball->position = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
  ball->stuck = true;
}
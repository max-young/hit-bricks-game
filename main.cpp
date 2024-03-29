// OpenGL是一个规范, 不同驱动开发商有不同的实现, 一个函数在不同的驱动里的位置不同, glad可以找到各自的位置, http://maxyoung.fun/blog/OpenGL%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE.html#glad
#include <glad/glad.h>
// GLFW提供渲染的基本接口, 例如创建窗口、上下文、接受用户输入等, http://maxyoung.fun/blog/OpenGL%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE.html#glfw
#include <GLFW/glfw3.h>

#include <iostream>

#include "Game.h"

using std::cout;
using std::endl;

// 保持窗口和framebuffer大小一致
void framebufferSizeCallback(GLFWwindow *window, int width, int height);
// 接受用户输入
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

constexpr unsigned int SCR_WIDTH = 1440;
constexpr unsigned int SCR_HEIGHT = 900;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Game Max(SCR_WIDTH, SCR_HEIGHT);

int main()
{
  // 初始化窗口配置
  glfwInit();
  // 这里基于OpenGL版本3.4, 将主版本和次版本都设置为3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // 明确告诉GLFW我们使用的是核心模式(Core-profile)。明确告诉GLFW我们需要使用核心模式意味着我们只能使用OpenGL功能的一个子集（没有我们已不再需要的向后兼容特性)
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // 如果使用的是Mac OS X系统，你还需要加下面这行代码到你的初始化代码中这些配置才能起作用
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // 创建窗口对象, 参数分别是长、宽、名称, 后面两个参数暂时忽略
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MaxGame", NULL, NULL);
  if (window == NULL)
  {
    cout << "Failed to create GLFW window" << endl;
    glfwTerminate();
    return -1;
  }
  // 设置这个窗口为上下文
  glfwMakeContextCurrent(window);
  // 回调函数, 注册这个函数，告诉GLFW我们希望每当窗口调整大小的时候调用这个函数
  // 这个函数将窗口大小和framebuffer大小保持一致
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glfwSetKeyCallback(window, keyCallback);

  // 调用OpenGL函数之前需要初始化glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    cout << "Failed to initialize GLAD" << endl;
    return -1;
  }

  Max.init();

  // 保持窗口打开, 接受用户输入, 不断绘制
  while (!glfwWindowShouldClose(window))
  {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // 渲染指令
    // 清空颜色缓冲并填充为深蓝绿色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    Max.render();

    Max.processInput(deltaTime);

    Max.update(deltaTime);

    // 将framebuffer的像素颜色值绘制到窗口
    glfwSwapBuffers(window);
    // 检查有没有触发事件
    glfwPollEvents();
  }

  // 释放资源
  glfwTerminate();
  return 0;
}

// 回调函数, 窗口大小改变时, 创建渲染窗口
void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
  // 渲染窗口的大小, glViewport函数前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）
  // 图形学里的viewport视口
  glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Max.keys[key] = true;
        else if (action == GLFW_RELEASE)
            Max.keys[key] = false;
    }
}

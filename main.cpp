// GLFW提供渲染的基本接口, 例如创建窗口、上下文、接受用户输入等, http://maxyoung.fun/blog/OpenGL%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE.html#glfw
#include <GLFW/glfw3.h>
// OpenGL是一个规范, 不同驱动开发商有不同的实现, 一个函数在不同的驱动里的位置不同, glad可以找到各自的位置, http://maxyoung.fun/blog/OpenGL%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE.html#glad

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

  return 0;
}

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const char WINDOW_LABEL[] = "EasyOpenGL by Nesb01t";

using namespace std;

/// <summary>
/// 初始化 glfw 并配置参数
/// </summary>
void initGlfw() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

/// <summary>
/// 创建窗口对象, 存放了窗口相关数据
/// </summary>
/// <returns></returns>
GLFWwindow* createWindowObj() {

    // todo
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_LABEL, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return (GLFWwindow*)-1;
    }
    glfwMakeContextCurrent(window);
    return window;
}

int initGlad() {
    // todo -> glfwGetProcAddress
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    return 1;
}

void initViewport(GLFWwindow* window) {
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        });
}

void mainThread(GLFWwindow* window) {
    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}

int main() {
    initGlfw();
    GLFWwindow* window = createWindowObj();
    initGlad();
    initViewport(window);
    mainThread(window);
    return 0;
}

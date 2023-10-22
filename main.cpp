#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// 窗口大小
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// 顶点着色器源码
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform float rotation;

    mat3 getRotationMatrix(float angle) {
        float s = sin(angle);
        float c = cos(angle);
        return mat3(
            c, -s, 0.0,
            s, c, 0.0,
            0.0, 0.0, 1.0
        );
    }

    void main()
    {
        mat3 rotationMatrix = getRotationMatrix(rotation);
        gl_Position = vec4(rotationMatrix * aPos, 1.0);
    }
)";

// 片段着色器源码
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    uniform float time; 
    void main()
    {
        float red = sin(time);
        float green = cos(time);
        float blue = 0.5 + 0.5 * sin(2.0 * time);

        FragColor = vec4(red, green, blue, 1.0);
    }
)";

int main() {
    // 初始化GLFW
    if (!glfwInit()) {
        std::cout << "GLFW initialization failed" << std::endl;
        return -1;
    }

    // 配置GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口对象
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Triangle", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 将窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);

    // 初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 创建顶点着色器对象
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // 检查顶点着色器是否编译成功
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "Failed to compile vertex shader:\n" << infoLog << std::endl;
        return -1;
    }

    // 创建片段着色器对象
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // 检查片段着色器是否编译成功
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "Failed to compile fragment shader:\n" << infoLog << std::endl;
        return -1;
    }

    // 创建着色器程序对象
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 检查着色器程序是否链接成功
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "Failed to link shader program:\n" << infoLog << std::endl;
        return -1;
    }

    // 删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 设置顶点数据
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.0f,  0.0f, 0.0f  // 原点坐标
    };

    // 创建顶点缓冲对象（VBO）和顶点数组对象（VAO）
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 绑定VAO和VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 解绑VAO和VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 渲染循环
    while (!glfwWindowShouldClose(window)) {
        // 处理输入事件
        glfwPollEvents();

        // 清空颜色缓冲
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 使用着色器程序
        glUseProgram(shaderProgram);

        // 获取当前时间
        float time = glfwGetTime();
        float rotation = time;
        int rotationLocation = glGetUniformLocation(shaderProgram, "rotation");
        glUniform1f(rotationLocation, rotation);
        
        int timeLoadtion = glGetUniformLocation(shaderProgram, "time");
        glUniform1f(timeLoadtion, time);

        // 绑定VAO
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 3, 1);  // 从索引3开始绘制一个点

        // 绘制三角形
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // 解绑VAO
        glBindVertexArray(0);

        // 交换缓冲区
        glfwSwapBuffers(window);
    }

    // 删除VAO和VBO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // 终止GLFW
    glfwTerminate();

    return 0;
}
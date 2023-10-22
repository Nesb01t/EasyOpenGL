#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// ���ڴ�С
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// ������ɫ��Դ��
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

// Ƭ����ɫ��Դ��
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
    // ��ʼ��GLFW
    if (!glfwInit()) {
        std::cout << "GLFW initialization failed" << std::endl;
        return -1;
    }

    // ����GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // �������ڶ���
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Triangle", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // �����ڵ�����������Ϊ��ǰ�̵߳���������
    glfwMakeContextCurrent(window);

    // ��ʼ��GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ����������ɫ������
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // ��鶥����ɫ���Ƿ����ɹ�
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "Failed to compile vertex shader:\n" << infoLog << std::endl;
        return -1;
    }

    // ����Ƭ����ɫ������
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // ���Ƭ����ɫ���Ƿ����ɹ�
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "Failed to compile fragment shader:\n" << infoLog << std::endl;
        return -1;
    }

    // ������ɫ���������
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // �����ɫ�������Ƿ����ӳɹ�
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "Failed to link shader program:\n" << infoLog << std::endl;
        return -1;
    }

    // ɾ����ɫ������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ���ö�������
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.0f,  0.0f, 0.0f  // ԭ������
    };

    // �������㻺�����VBO���Ͷ����������VAO��
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // ��VAO��VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // ���ö�������ָ��
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ���VAO��VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // ��Ⱦѭ��
    while (!glfwWindowShouldClose(window)) {
        // ���������¼�
        glfwPollEvents();

        // �����ɫ����
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ʹ����ɫ������
        glUseProgram(shaderProgram);

        // ��ȡ��ǰʱ��
        float time = glfwGetTime();
        float rotation = time;
        int rotationLocation = glGetUniformLocation(shaderProgram, "rotation");
        glUniform1f(rotationLocation, rotation);
        
        int timeLoadtion = glGetUniformLocation(shaderProgram, "time");
        glUniform1f(timeLoadtion, time);

        // ��VAO
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 3, 1);  // ������3��ʼ����һ����

        // ����������
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // ���VAO
        glBindVertexArray(0);

        // ����������
        glfwSwapBuffers(window);
    }

    // ɾ��VAO��VBO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // ��ֹGLFW
    glfwTerminate();

    return 0;
}
/*** 
 * @Author: pananfly
 * @Date: 2022-04-11 16:49:58
 * @LastEditTime: 2022-04-11 17:10:55
 * @LastEditors: pananfly
 * @Description: 
 * @FilePath: \Triangle\triangle3.cpp
 * @pananfly
 */
/*** 
 * @Author: pananfly
 * @Date: 2022-04-11 16:06:36
 * @LastEditTime: 2022-04-11 16:45:21
 * @LastEditors: pananfly
 * @Description: 
 * @FilePath: \Triangle\triangle2.cpp
 * @pananfly
 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int VERTEX_INDEX_ID = 0;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0"; // location = VERTEX_INDEX_ID
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); // 创建顶点着色器
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);// 加载着色器源码（着色器对象，着色器源码字符串数量个数，着色器源码指向的地址，NULL）
    glCompileShader(vertexShader); // 编译顶点着色器

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // 创建片段着色器
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);// 加载着色器源码
    glCompileShader(fragmentShader);// 编译着色器源码

    unsigned int shaderProgram = glCreateProgram(); // 创建着色器程序
    glAttachShader(shaderProgram, vertexShader); // 附加顶点着色器到着色器程序
    glAttachShader(shaderProgram, fragmentShader); // 附加片段着色器到着色器程序
    glLinkProgram(shaderProgram); // 链接着色器程序

    glDeleteShader(vertexShader); // 这时候我们不需要顶点着色器了，我们可以释放掉它
    glDeleteShader(fragmentShader); // 同上

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO); // 创建顶点数组
    glBindVertexArray(VAO); // 绑定顶点数组
    glGenBuffers(1, &VBO); // 创建顶点缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定顶点缓冲
    glGenBuffers(1, &EBO); // 创建下标缓冲
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 绑定下标缓冲

    float vertices[] = {
        0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        // 第二个三角形
        0.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 把顶点数据复制到缓冲中给opengl使用
    int indeces[] = {
        0, 1, 2,
        // 第二个三角形
        3, 4, 5
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW); // 把元素下标复制到缓冲中给opengl使用

    glVertexAttribPointer(VERTEX_INDEX_ID, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)nullptr); // 告诉opengl如何解析顶点数据
    glEnableVertexAttribArray(VERTEX_INDEX_ID); // 使能对应顶点着色器id，默认关闭
    glUseProgram(shaderProgram); // 使用着色器程序

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glDrawArrays(GL_TRIANGLES, 0, 6); // 画三角形，从下标0开始，画多少个顶点
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 通过画顶点元素的形式画三角形
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        // glfwPollEvents();
        glfwWaitEvents();
    }

    // 释放资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
    {
        glfwSetWindowShouldClose(window, true);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
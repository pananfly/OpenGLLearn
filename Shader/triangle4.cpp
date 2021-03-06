/*** 
 * @Author: pananfly
 * @Date: 2022-04-12 17:30:14
 * @LastEditTime: 2022-04-13 11:50:08
 * @LastEditors: pananfly
 * @Description: 
 * @FilePath: \Shader\triangle4.cpp
 * @pananfly
 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <string>
#include "SimpleShader.h"
#include "LocalShaderReader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int VERTEX_POS_INDEX_ID = 0;
const unsigned int VERTEX_COLOR_INDEX_ID = 1;


int main(int argc, const char* argv[])
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
    
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes  << ", path: " << argv[0] << std::endl;

    GLShader::LocalShaderReader shaderReader;
    std::string vertexShaderCode, fragmentShaderCode;
    bool readRet = shaderReader.LoadShaderSource("../../../shader.vs", vertexShaderCode);
    readRet = shaderReader.LoadShaderSource("../../../shader.fs", fragmentShaderCode);
    GLShader::SimpleShader shader(vertexShaderCode.c_str(), fragmentShaderCode.c_str());

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO); // ??????????????????
    glBindVertexArray(VAO); // ??????????????????
    glGenBuffers(1, &VBO); // ??????????????????
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // ??????????????????
    glGenBuffers(1, &EBO); // ??????????????????
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // ??????????????????

    float vertices[] = {
        // ??????            // ??????
        0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,   
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,    
        // ??????????????????
        0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f    
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // ????????????????????????????????????opengl??????
    int indeces[] = {
        0, 1, 2,
        // ??????????????????
        3, 4, 5
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW); // ??????????????????????????????????????????opengl??????

    glVertexAttribPointer(VERTEX_POS_INDEX_ID, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)nullptr); // ??????opengl???????????????????????? ??????6???float????????????0
    glEnableVertexAttribArray(VERTEX_POS_INDEX_ID); // ???????????????????????????id???????????????
    glVertexAttribPointer(VERTEX_COLOR_INDEX_ID, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float))); // ??????opengl???????????????????????? ??????6???float????????????3???float
    glEnableVertexAttribArray(VERTEX_COLOR_INDEX_ID); // ???????????????????????????id???????????????

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

        shader.Use(); // ?????????????????????

        glBindVertexArray(VAO); // ??????????????????
        // glDrawArrays(GL_TRIANGLES, 0, 6); // ????????????????????????0???????????????????????????
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // ??????????????????????????????????????????
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        // glfwPollEvents();
        glfwWaitEvents();
    }

    // ????????????
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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
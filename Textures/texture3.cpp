/*** 
 * @Author: pananfly
 * @Date: 2022-04-14 10:24:31
 * @LastEditTime: 2022-04-15 09:32:43
 * @LastEditors: pananfly
 * @Description: 
 * @FilePath: \Textures\texture3.cpp
 * @pananfly
 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <string>
#include "SimpleShader.h"
#include "LocalShaderReader.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int VERTEX_POS_INDEX_ID = 0;
const unsigned int VERTEX_COLOR_INDEX_ID = 1;
const unsigned int VERTEX_TEXTURE_INDEX_ID = 2;

float mixValue = 0.2f;
float translateX = 0.5f;
float translateY = -0.5f;

int main(int argc, const char* argv[])
{

    // test glm
    // 齐次坐标
    glm::vec4 vecTest(1.0f, 0.0f, 0.0f, 1.0f);
    // 单位4X4矩阵 glm v0.9.9开始要进行初始化
    glm::mat4 traslateTest = glm::mat4(1.0);
    // 位移 
    traslateTest = glm::translate(traslateTest, glm::vec3(1.0f, 1.0f, 0.0f));
    vecTest = traslateTest * vecTest;
    std::cout << "glm test, x: " << vecTest.x << ", y: " << vecTest.y << ", z: " << vecTest.z << std::endl;

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
    bool readRet = shaderReader.LoadShaderSource("../../../texture3.vs", vertexShaderCode);
    readRet = shaderReader.LoadShaderSource("../../../texture2.fs", fragmentShaderCode);
    GLShader::SimpleShader shader(vertexShaderCode.c_str(), fragmentShaderCode.c_str());

    uint32_t VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO); // 创建顶点数组
    glBindVertexArray(VAO); // 绑定顶点数组
    glGenBuffers(1, &VBO); // 创建顶点缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定顶点缓冲
    glGenBuffers(1, &EBO); // 创建索引缓冲
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 绑定索引缓冲

    float vertices[] = {
        // 位置            // 颜色           // 纹理坐标
         1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left  
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 把顶点数据复制到缓冲中给opengl使用
    int indeces[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW); // 把元素下标复制到索引缓冲中给opengl使用

    glVertexAttribPointer(VERTEX_POS_INDEX_ID, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)nullptr); // 告诉opengl如何解析顶点数据 vec3 间隔8个float偏移量为0
    glEnableVertexAttribArray(VERTEX_POS_INDEX_ID); // 使能对应顶点着色器id，默认关闭
    glVertexAttribPointer(VERTEX_COLOR_INDEX_ID, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float))); // 告诉opengl如何解析顶点数据 vec3 间隔8个float偏移量为3个float
    glEnableVertexAttribArray(VERTEX_COLOR_INDEX_ID); // 使能对应顶点着色器id，默认关闭
    glVertexAttribPointer(VERTEX_TEXTURE_INDEX_ID, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float))); // 告诉opengl如何解析顶点数据 vec2 间隔8个float偏移量为6个float
    glEnableVertexAttribArray(VERTEX_TEXTURE_INDEX_ID); // 使能对应顶点着色器id，默认关闭

    uint32_t TEXTURE, TEXTURE2;
    // 创建纹理
    glGenTextures(1, &TEXTURE);
    // 绑定第一个纹理对象
    glBindTexture(GL_TEXTURE_2D, TEXTURE);
    // 为当前绑定的纹理对象设置环绕和过滤方式
    // 环绕方式
    // GL_REPEAT	对纹理的默认行为。重复纹理图像。
    // GL_MIRRORED_REPEAT	和GL_REPEAT一样，但每次重复图片是镜像放置的。
    // GL_CLAMP_TO_EDGE	纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。
    // GL_CLAMP_TO_BORDER	超出的坐标为用户指定的边缘颜色。
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // 过滤方式
    // GL_NEAREST（也叫邻近过滤，Nearest Neighbor Filtering）是OpenGL默认的纹理过滤方式。当设置为GL_NEAREST的时候，OpenGL会选择中心点最接近纹理坐标的那个像素。下图中你可以看到四个像素，加号代表纹理坐标。左上角那个纹理像素的中心距离纹理坐标最近，所以它会被选择为样本颜色
    // GL_LINEAR（也叫线性过滤，(Bi)linear Filtering）它会基于纹理坐标附近的纹理像素，计算出一个插值，近似出这些纹理像素之间的颜色。一个纹理像素的中心距离纹理坐标越近，那么这个纹理像素的颜色对最终的样本颜色的贡献越大。下图中你可以看到返回的颜色是邻近像素的混合色
    // GL_NEAREST_MIPMAP_NEAREST	使用最邻近的多级渐远纹理来匹配像素大小，并使用邻近插值进行纹理采样
    // GL_LINEAR_MIPMAP_NEAREST	使用最邻近的多级渐远纹理级别，并使用线性插值进行采样
    // GL_NEAREST_MIPMAP_LINEAR	在两个最匹配像素大小的多级渐远纹理之间进行线性插值，使用邻近插值进行采样
    // GL_LINEAR_MIPMAP_LINEAR	在两个邻近的多级渐远纹理之间使用线性插值，并使用线性插值进行采样
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 翻转图像y轴使得获取到的图像方向为正向向上
    stbi_set_flip_vertically_on_load(true);
    // 获取纹理图片数据 宽度、高度、颜色通道个数
    int32_t textureWidth, textureHeight, textuteNrChannels;
    uint8_t* textureData = stbi_load("../../../container.jpg", &textureWidth, &textureHeight, &textuteNrChannels, 0);
    if(textureData)
    {
        // 附加纹理图像到绑定的当前纹理对象
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        // 生成多级渐远纹理
        glGenerateMipmap(GL_TEXTURE_2D);
        // 释放纹理图像内存
        stbi_image_free(textureData);
    }
    
    // 创建纹理
    glGenTextures(1, &TEXTURE2);
    // 绑定第二个纹理对象
    glBindTexture(GL_TEXTURE_2D, TEXTURE2);
    // 设置环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 设置过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    textureData = stbi_load("../../../awesomeface.png", &textureWidth, &textureHeight, &textuteNrChannels, 0);
    if(textureData)
    {
        // 附加纹理图像到绑定的当前纹理对象
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        // 生成多级渐远纹理
        glGenerateMipmap(GL_TEXTURE_2D);
        // 释放纹理图像内存
        stbi_image_free(textureData);
    }
    shader.Use(); // 使用着色器程序
    shader.SetInt("ourTexture", 0); // 设置对应片元着色器中的纹理为对应的第一个纹理
    shader.SetInt("ourTexture2", 1); // 设置对应片元着色器中的纹理为对应的第二个纹理

    // 初始化单位矩阵
    // glm::mat4 shaderTrasform = glm::mat4(1.0);
    // // 因为是在xy轴平面上，所以设置沿着z轴旋转，这里radians代表旋转是有弧度制的
    // shaderTrasform = glm::rotate(shaderTrasform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // // 每个轴缩放0.5倍
    // shaderTrasform = glm::scale(shaderTrasform, glm::vec3(0.5f, 0.5f, 0.5f));
    // 第二个参数是多少个矩阵 
    // 第三个参数是是否进行转置就是说交换我们矩阵的行和列,OpenGL开发者通常使用一种内部矩阵布局，叫做列主序(Column-major Ordering)布局。GLM的默认布局就是列主序，所以并不需要转置矩阵
    // shader.SetMatrix4fv("transform", 1, false, glm::value_ptr(shaderTrasform));
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


        shader.Use(); // 使用着色器程序
                // 激活第一个纹理
        glActiveTexture(GL_TEXTURE0);
        // 绑定第一个纹理
        glBindTexture(GL_TEXTURE_2D, TEXTURE);
        // 激活第二个纹理
        glActiveTexture(GL_TEXTURE1);
        // 绑定纹理2
        glBindTexture(GL_TEXTURE_2D, TEXTURE2);
        glBindVertexArray(VAO); // 绑定顶点数组
        // 设置片元着色器的值
        shader.SetFloat("mixValue", mixValue);
        glm::mat4 shaderTrasform = glm::mat4(1.0);
        shaderTrasform = glm::translate(shaderTrasform, glm::vec3(translateX, translateY, 0.0f));
        shaderTrasform = glm::rotate(shaderTrasform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        // 每个轴缩放0.5倍
        shaderTrasform = glm::scale(shaderTrasform, glm::vec3(0.5f, 0.5f, 0.5f));
        shader.SetMatrix4fv("transform", 1, false, glm::value_ptr(shaderTrasform));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 通过画顶点元素的形式画三角形

        shaderTrasform = glm::mat4(1.0);
        // 不断的放大缩小 0-1, 注意sin值为负值是会有翻转的情况，所以加上绝对值进行矫正
        double scaleValue = std::min(std::abs(std::sin(glfwGetTime())), 0.5);
        // shaderTrasform = glm::translate(shaderTrasform, glm::vec3(-0.5f, 0.5f, 0.0f));
        shaderTrasform = glm::scale(shaderTrasform, glm::vec3(scaleValue, scaleValue, scaleValue));
        shader.SetMatrix4fv("transform", 1, false, glm::value_ptr(shaderTrasform));

        // glDrawArrays(GL_TRIANGLES, 0, 6); // 画三角形，从下标0开始，画多少个顶点
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 通过画顶点元素的形式画三角形
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
        // WaitEvents会卡住等待信号
        // glfwWaitEvents();
    }

    // 释放资源
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
    else if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue = std::min(mixValue += 0.001f, 1.0f);
        translateY = std::min(translateY += 0.001f, 0.5f);
    }
    else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue = std::max(mixValue -= 0.001f, 0.0f);
        translateY = std::max(translateY -= 0.001f, -0.5f);
    }
    else if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        translateX = std::max(translateX -= 0.001f, -0.5f);
    }
    else if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        translateX = std::min(translateX += 0.001f, 0.5f);
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
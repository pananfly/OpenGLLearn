/*** 
 * @Author: pananfly
 * @Date: 2022-04-12 14:16:13
 * @LastEditTime: 2022-04-14 10:49:23
 * @LastEditors: pananfly
 * @Description: 
 * @FilePath: \Texturesc:\WorkSpaces\c_cplus_space\OpenGLLearn\libs\opengl\shader\SimpleShader.cpp
 * @pananfly
 */
#include "SimpleShader.h"
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <array>


const int32_t INFO_LOG_SIZE = 512;

GLShader::SimpleShader::SimpleShader(const char * vertexShaderCode, const char * fragmentShaderCode):
ID(0)
{
    int32_t success;
    std::array<char, INFO_LOG_SIZE> infoLog = {0};
    // 创建顶点着色器
    uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 加载顶点着色器资源
    glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
    // 编译顶点着色器
    glCompileShader(vertexShader);
    // 获取编译状态是否成功
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, INFO_LOG_SIZE, nullptr, infoLog.data());
        std::cout << "ERROR:: Vertex shader comiple source failed: " << infoLog.data() << std::endl;
        return;
    }
    // 创建片段着色器
    uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // 加载片段着色器资源
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
    // 编译片段着色器
    glCompileShader(fragmentShader);
    // 获取编译状态是否成功
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, INFO_LOG_SIZE, nullptr, infoLog.data());
        std::cout << "ERROR:: Fragment shader comiple source failed: " << infoLog.data() << std::endl;
        return;
    }
    // 创建着色器程序
    ID = glCreateProgram();
    // 附着顶点着色器到着色器程序
    glAttachShader(ID, vertexShader);
    // 附着片段着色器到着色器程序
    glAttachShader(ID, fragmentShader);
    // 链接着色器程序
    glLinkProgram(ID);
    // 获取着色器程序状态是否成功
    glGetShaderiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(ID, INFO_LOG_SIZE, nullptr, infoLog.data());
        std::cout << "ERROR:: Program of shader failed: " << infoLog.data() << std::endl;
        return;
    }
    // 删除着色器，已经链接到着色器程序就不再需要了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLShader::SimpleShader::~SimpleShader()
{
    if(ID)
    {
        glDeleteProgram(ID);
    }
}

void GLShader::SimpleShader::Use() const
{
    if(ID)
    {
        glUseProgram(ID);
    }
}

void GLShader::SimpleShader::SetBool(const std::string &name, bool value) const
{
    if(ID) 
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
}

void GLShader::SimpleShader::SetInt(const std::string &name, int value) const
{
    if(ID) 
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
}

void GLShader::SimpleShader::SetFloat(const std::string &name, float value) const
{
    if(ID) 
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
}

void GLShader::SimpleShader::Set2Float(const std::string &name, float v1, float v2) const
{
    if(ID) 
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), v1, v2);
    }
}

void GLShader::SimpleShader::Set3Float(const std::string &name, float v1, float v2, float v3) const
{
    if(ID) 
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
    }
}

void GLShader::SimpleShader::Set4Float(const std::string &name, float v1, float v2, float v3, float v4) const
{
    if(ID) 
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3, v4);
    }
}

void GLShader::SimpleShader::SetMatrix4fv(const std::string &name, int matrixSize, bool transpose, float* value) const
{
    if(ID) 
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), matrixSize, transpose, value);
    }
}
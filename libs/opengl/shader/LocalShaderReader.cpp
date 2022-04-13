/*** 
 * @Author: pananfly
 * @Date: 2022-04-13 09:43:05
 * @LastEditTime: 2022-04-13 09:57:24
 * @LastEditors: pananfly
 * @Description: 
 * @FilePath: \Shaderc:\WorkSpaces\c_cplus_space\OpenGLLearn\libs\opengl\shader\LocalShaderReader.cpp
 * @pananfly
 */
#include "LocalShaderReader.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>

bool GLShader::LocalShaderReader::LoadShaderSource(const char *shaderPath, std::string &shaderSource)
{
    if(!shaderPath || !std::strlen(shaderPath))
    {
        std::cout << "ERROR: Local shader reader load source path is empty or null " << std::endl;
        return false;
    }
    std::ifstream shaderFile;
    std::cout << "shaderPath: " << shaderPath << std::endl;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // 打开文件
        shaderFile.open(shaderPath);
        std::stringstream vShaderStream;
        // 读取文件内容到流中
        vShaderStream << shaderFile.rdbuf();
        // 关闭文件处理
        shaderFile.close();
        // 转换数据流到字符串
        shaderSource = vShaderStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR: Local shader reader load file to read failed: " << e.what() << std::endl;
        return false;
    }
    return true;
}
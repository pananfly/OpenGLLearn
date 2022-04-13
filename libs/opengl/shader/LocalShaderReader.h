/*** 
 * @Author: pananfly
 * @Date: 2022-04-13 09:43:05
 * @LastEditTime: 2022-04-13 10:00:00
 * @LastEditors: pananfly
 * @Description: 
 * @FilePath: \Shaderc:\WorkSpaces\c_cplus_space\OpenGLLearn\libs\opengl\shader\LocalShaderReader.h
 * @pananfly
 */
#ifndef SHDER_LOCAL_SHADER_LOADER_H
#define SHDER_LOCAL_SHADER_LOADER_H

#include <string>

namespace GLShader {
    class LocalShaderReader
    {
    public:
        bool LoadShaderSource(const char* shaderPath, std::string &shaderSource);    
    };
}  // namespace GLShader
#endif
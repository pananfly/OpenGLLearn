/*** 
 * @Author: pananfly
 * @Date: 2022-04-12 14:16:02
 * @LastEditTime: 2022-04-14 10:48:00
 * @LastEditors: pananfly
 * @Description: 
 * @FilePath: \Texturesc:\WorkSpaces\c_cplus_space\OpenGLLearn\libs\opengl\shader\SimpleShader.h
 * @pananfly
 */
#ifndef SHDER_SIMPLE_SHADER_H
#define SHDER_SIMPLE_SHADER_H
#include <glad/glad.h>
#include <cstdint>
#include <cstring>
#include <string>

namespace GLShader {
    class SimpleShader 
    {
    public:
        SimpleShader(const char * vertexShaerCode, const char * fragmentShaderCode);
        ~SimpleShader();
        void Use() const;
        void SetBool(const std::string &name, bool value) const;
        void SetInt(const std::string &name, int value) const;
        void SetFloat(const std::string &name, float value) const;
        void Set2Float(const std::string &name, float v1, float v2) const;
        void Set3Float(const std::string &name, float v1, float v2, float v3) const;
        void Set4Float(const std::string &name, float v1, float v2, float v3, float v4) const;
        void SetMatrix4fv(const std::string &name, int matrixSize, bool transpose, float* value) const;
    private:
        uint32_t ID; // 着色器程序id
    };
}  // namespace GLShader

#endif
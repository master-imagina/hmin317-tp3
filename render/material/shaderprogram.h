#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>


struct ShaderProgram
{
    ShaderProgram();
    ~ShaderProgram();

    std::string vertexShaderSource;
    std::string geometryShaderSource;
    std::string fragmentShaderSource;
};

#endif // SHADERPROGRAM_H

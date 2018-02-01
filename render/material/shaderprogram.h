#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>

class QDataStream;


struct ShaderProgram
{
    ShaderProgram();
    ~ShaderProgram();

    std::string vertexShaderSource;
    std::string geometryShaderSource;
    std::string fragmentShaderSource;
};


QDataStream &operator<<(QDataStream &os, const ShaderProgram &program);
QDataStream &operator>>(QDataStream &os, ShaderProgram &program);

#endif // SHADERPROGRAM_H

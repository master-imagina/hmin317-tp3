#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>

#include <QByteArray>


struct ShaderProgram
{
    ShaderProgram();
    ~ShaderProgram();

    QByteArray vertexShaderSource;
    QByteArray geometryShaderSource;
    QByteArray fragmentShaderSource;
};

#endif // SHADERPROGRAM_H

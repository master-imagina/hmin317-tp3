#include "shaderprogram.h"

#include <QDataStream>


ShaderProgram::ShaderProgram() :
    vertexShaderSource(),
    geometryShaderSource(),
    fragmentShaderSource()
{}

ShaderProgram::~ShaderProgram()
{}


////////////////////// Functions //////////////////////

QDataStream &operator<<(QDataStream &os, const ShaderProgram &program)
{
    os << QString::fromStdString(program.vertexShaderSource);
    os << QString::fromStdString(program.geometryShaderSource);
    os << QString::fromStdString(program.fragmentShaderSource);
}

QDataStream &operator>>(QDataStream &os, ShaderProgram &program)
{
    QString vertexShaderSource;
    QString geometryShaderSource;
    QString fragmentShaderSource;

    os >> vertexShaderSource;
    os >> geometryShaderSource;
    os >> fragmentShaderSource;

    program.vertexShaderSource = vertexShaderSource.toStdString();
    program.geometryShaderSource = geometryShaderSource.toStdString();
    program.fragmentShaderSource = fragmentShaderSource.toStdString();
}

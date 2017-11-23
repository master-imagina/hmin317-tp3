#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>

#include "core/aliases_memory.h"


struct ShaderProgram
{
    ShaderProgram();
    ~ShaderProgram();

    std::string vertexShaderSource;
    std::string geometryShaderSource;
    std::string fragmentShaderSource;
};

uptr<ShaderProgram> shaderProgramFromSource(const std::string &vertShaderSource,
                                            const std::string &geomShaderSource,
                                            const std::string &fragShaderSource);

#endif // SHADERPROGRAM_H

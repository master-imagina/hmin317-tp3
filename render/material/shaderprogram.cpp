#include "shaderprogram.h"


////////////////////// ShaderProgram //////////////////////

ShaderProgram::ShaderProgram() :
    vertexShaderSource(),
    geometryShaderSource(),
    fragmentShaderSource()
{}

ShaderProgram::~ShaderProgram()
{}


////////////////////// Functions //////////////////////

uptr<ShaderProgram> shaderProgramFromSource(const std::string &vertShaderSource,
                                            const std::string &geomShaderSource,
                                            const std::string &fragShaderSource)
{
    auto ret = std::make_unique<ShaderProgram>();

    ret->vertexShaderSource = vertShaderSource;
    ret->geometryShaderSource = geomShaderSource;
    ret->fragmentShaderSource = fragShaderSource;

    return ret;
}

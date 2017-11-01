#ifndef SHADERUTILS_H
#define SHADERUTILS_H

#include "core/aliases_memory.h"

#include "shaderprogram.h"


uptr<ShaderProgram> shaderProgramFromFile(const std::string &vertFilePath,
                                          const std::string &geomFilePath,
                                          const std::string &fragFilePath);

#endif // SHADERUTILS_H

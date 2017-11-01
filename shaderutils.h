#ifndef SHADERUTILS_H
#define SHADERUTILS_H

#include <memory>

#include <QFile>

#include "renderer/aliases_memory.h"
#include "renderer/shaderprogram.h"


uptr<ShaderProgram> shaderProgramFromFile(const std::string &vertFilePath,
                                          const std::string &geomFilePath,
                                          const std::string &fragFilePath);

#endif // SHADERUTILS_H

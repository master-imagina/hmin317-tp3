#ifndef RENDERASSETS_H
#define RENDERASSETS_H

#include <QImage>

#include "core/aliases_memory.h"

#include "render/material/shaderprogram.h"

struct aiScene;

namespace Assimp {
class Importer;
} // namespace Assimp


uptr<ShaderProgram> shaderProgramFromFile(const std::string &vertFilePath,
                                          const std::string &fragFilePath);

uptr<ShaderProgram> shaderProgramFromFile(const std::string &vertFilePath,
                                          const std::string &geomFilePath,
                                          const std::string &fragFilePath);

QImage imageFromFile(const std::string &path);

const aiScene *assimpSceneFromFile(const std::string &path, Assimp::Importer &importer);

#endif // RENDERASSETS_H

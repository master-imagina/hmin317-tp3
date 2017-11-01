#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <map>

#include "core/aliases_int.h"

class ShaderProgram;


class ShaderManager
{
public:
    ShaderManager();

    void addShaderProgram(ShaderProgram *shaderProgram, uint32 shaderId);

    uint32 shaderIdForShaderProgram(ShaderProgram *shaderProgram);

private:
    std::map<ShaderProgram *, uint32> m_shaderProgramToId;
};

#endif // SHADERMANAGER_H

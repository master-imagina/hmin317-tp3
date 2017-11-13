#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <unordered_map>

#include "core/aliases_int.h"

class GLWrapper;
class ShaderProgram;


class ShaderManager
{
public:
    ShaderManager();

    void addShaderProgram(ShaderProgram *shaderProgram, uint32 shaderId);

    uint32 shaderIdForShaderProgram(ShaderProgram *shaderProgram) const;

    void cleanup(GLWrapper &glWrapper);

private:
    std::unordered_map<ShaderProgram *, uint32> m_shaderProgramToId;
};

#endif // SHADERMANAGER_H

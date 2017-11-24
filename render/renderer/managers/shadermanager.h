#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <unordered_map>

#include "core/aliases_int.h"
#include "core/aliases_memory.h"

#include "render/renderer/glshaderprogram.h"

class GLWrapper;
class ShaderProgram;


class ShaderManager
{
public:
    ShaderManager();

    GLShaderProgram *addShaderProgram(ShaderProgram *shaderProgram, GLWrapper &glWrapper);
    bool isAllocated(ShaderProgram *shaderProgram) const;

    GLShaderProgram *get(ShaderProgram *shaderProgram) const;

    void cleanup(GLWrapper &glWrapper);

private:
    std::unordered_map<ShaderProgram *, uptr<GLShaderProgram>> m_shaderProgramToId;
};

#endif // SHADERMANAGER_H

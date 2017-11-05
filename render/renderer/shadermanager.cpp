#include "shadermanager.h"

#include "../material/shaderprogram.h"

#include "glwrapper.h"


ShaderManager::ShaderManager() :
    m_shaderProgramToId()
{}

void ShaderManager::addShaderProgram(ShaderProgram *shaderProgram, uint32 shaderId)
{
    const auto isHandled = m_shaderProgramToId.find(shaderProgram);

    if (isHandled == m_shaderProgramToId.cend()) {
        m_shaderProgramToId.insert({shaderProgram, shaderId});
    }
}

uint32 ShaderManager::shaderIdForShaderProgram(ShaderProgram *shaderProgram) const
{
    const auto shaderProgramFound = m_shaderProgramToId.find(shaderProgram);

    uint32 ret = 0;

    if (shaderProgramFound != m_shaderProgramToId.end()) {
        ret = shaderProgramFound->second;
    }

    return ret;
}

void ShaderManager::cleanup(GLWrapper &glWrapper)
{
    for (auto shaderProgramAndId : m_shaderProgramToId) {
        glWrapper.destroyShaderProgram(shaderProgramAndId.second);
    }
}

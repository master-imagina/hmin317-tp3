#include "shadermanager.h"

#include "shaderprogram.h"


ShaderManager::ShaderManager() :
    m_shaderProgramToId()
{}

void ShaderManager::addShaderProgram(ShaderProgram *shaderProgram, unsigned int shaderId)
{
    const auto isHandled = m_shaderProgramToId.find(shaderProgram);

    if (isHandled == m_shaderProgramToId.cend()) {
        m_shaderProgramToId.insert({shaderProgram, shaderId});
    }
}

unsigned int ShaderManager::shaderIdForShaderProgram(ShaderProgram *shaderProgram)
{
    const auto shaderProgramFound = m_shaderProgramToId.find(shaderProgram);

    unsigned int ret = 0;

    if (shaderProgramFound != m_shaderProgramToId.end()) {
        ret = shaderProgramFound->second;
    }

    return ret;
}

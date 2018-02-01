#include "shadermanager.h"

#include <iostream>

#include "render/material/shaderprogram.h"

#include "render/renderer/glwrapper.h"


ShaderManager::ShaderManager() :
    m_shaderProgramToId()
{}

GLShaderProgram *ShaderManager::addShaderProgram(ShaderProgram *shaderProgram, GLWrapper &glWrapper)
{
    const auto isHandled = m_shaderProgramToId.find(shaderProgram);

    if (isHandled != m_shaderProgramToId.cend()) {
        std::cout << "[WARNING] ShaderManager::addShaderProgram(): shader program already exists"
                  << std::endl;
    }

    auto glProgram = std::make_unique<GLShaderProgram>();
    GLShaderProgram *ret = glProgram.get();

    // Create GL shader program
    glWrapper.createShaderProgram(*ret, *shaderProgram);

    m_shaderProgramToId.emplace(shaderProgram, std::move(glProgram));

    return ret;
}

bool ShaderManager::isAllocated(ShaderProgram *shaderProgram) const
{
    const GLShaderProgram *glProgram = get(shaderProgram);

    return glProgram != nullptr;
}

GLShaderProgram *ShaderManager::get(ShaderProgram *shaderProgram) const
{
    auto shaderProgramFound = m_shaderProgramToId.find(shaderProgram);

    GLShaderProgram *ret = nullptr;

    if (shaderProgramFound != m_shaderProgramToId.end()) {
        ret = shaderProgramFound->second.get();
    }

    return ret;
}

void ShaderManager::cleanup(GLWrapper &glWrapper)
{
    for (auto &programAndGlId : m_shaderProgramToId) {
        glWrapper.destroyShaderProgram(*programAndGlId.second.get());
    }
}

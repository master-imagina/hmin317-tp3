#include "shadermanager.h"

#include <iostream>

#include "render/material/shaderprogram.h"

#include "render/renderer/glwrapper.h"


ShaderManager::ShaderManager() :
    m_shaderProgramToId(),
    m_shaderPrograms()
{}

GLShaderProgram *ShaderManager::addShaderProgram(ShaderProgram *shaderProgram, GLWrapper &glWrapper)
{
    const auto isHandled = m_shaderProgramToId.find(shaderProgram);

    if (isHandled != m_shaderProgramToId.cend()) {
        std::cerr << "ShaderManager::addShaderProgram(): shader program already exists"
                  << std::endl;
    }

    auto glProgram = std::make_unique<GLShaderProgram>();
    GLShaderProgram *ret = glProgram.get();

    // Create GL shader program
    glWrapper.createShaderProgram(*ret, *shaderProgram);

    m_shaderPrograms.emplace_back(std::move(glProgram));

    m_shaderProgramToId.insert({shaderProgram, ret});

    return ret;
}

bool ShaderManager::isAllocated(ShaderProgram *shaderProgram) const
{
    const GLShaderProgram *glProgram = get(shaderProgram);

    return glProgram != nullptr;
}

GLShaderProgram *ShaderManager::get(ShaderProgram *shaderProgram) const
{
    const auto shaderProgramFound = m_shaderProgramToId.find(shaderProgram);

    GLShaderProgram *ret = nullptr;

    if (shaderProgramFound != m_shaderProgramToId.end()) {
        ret = shaderProgramFound->second;
    }

    return ret;
}

void ShaderManager::cleanup(GLWrapper &glWrapper)
{
    for (auto &program : m_shaderPrograms) {
        glWrapper.destroyShaderProgram(*program.get());
    }
}

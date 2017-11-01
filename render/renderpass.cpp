#include "renderpass.h"

#include <algorithm>

#include "shaderprogram.h"


int RenderPass::RESERVE_PARAM_COUNT = 8;


RenderPass::RenderPass(const std::string &name) :
    m_name(name),
    m_shaderProgram(),
    m_params()
{
    if (RESERVE_PARAM_COUNT > MAX_PARAM_COUNT) {
        RESERVE_PARAM_COUNT = MAX_PARAM_COUNT;
    }

    m_params.reserve(RESERVE_PARAM_COUNT);

}

RenderPass::~RenderPass()
{}

std::string RenderPass::name() const
{
    return m_name;
}

void RenderPass::setName(const std::string &name)
{
    if (m_name != name) {
        m_name = name;
    }
}

ShaderProgram *RenderPass::shaderProgram() const
{
    return m_shaderProgram.get();
}

void RenderPass::setShaderProgram(uptr<ShaderProgram> &&program)
{
    if (m_shaderProgram.get() != program.get()) {
        m_shaderProgram = std::move(program);
    }
}

void RenderPass::addParam(const ShaderParam &param)
{
    auto paramFound = std::find(m_params.begin(), m_params.end(), param);

    if (paramFound == m_params.end()) {
        m_params.emplace_back(param);
    }
}

void RenderPass::removeParam(const ShaderParam &param)
{
    m_params.erase(std::remove(m_params.begin(), m_params.end(), param),
                   m_params.end());
}

std::vector<ShaderParam> RenderPass::params() const
{
    return m_params;
}

void RenderPass::clearParams()
{
    m_params.clear();
}

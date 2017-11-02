#include "renderpass.h"

#include <algorithm>
#include <iostream>

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

ShaderParam *RenderPass::addParam(const std::string &name, const QVariant &value)
{
    auto paramFound = std::find_if(m_params.begin(), m_params.end(),
                                  [name] (const uptr<ShaderParam> &param) {
        return param->name == name;
    });

    if (paramFound != m_params.end()) {
        std::cerr << "RenderPass : param \"" << name << "\" already exists"
                  << std::endl;
        return nullptr;
    }

    // Create the shader param and return a handle to it
    auto param = std::make_unique<ShaderParam>(name, value);
    ShaderParam *ret = param.get();

    m_params.emplace_back(std::move(param));

    return ret;
}

void RenderPass::removeParam(ShaderParam *param)
{
    m_params.erase(std::remove_if(m_params.begin(), m_params.end(),
                                  [param]
                                  (const uptr<ShaderParam> &p) {
                        return p.get() == param;
                   }),
                   m_params.end());
}

const uptr_vector<ShaderParam> &RenderPass::params() const
{
    return m_params;
}

void RenderPass::clearParams()
{
    m_params.clear();
}

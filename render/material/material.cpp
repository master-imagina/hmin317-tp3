#include "material.h"

#include <algorithm>
#include <iostream>

#include "render/material/renderpass.h"
#include "render/material/shaderparam.h"
#include "render/material/shaderutils.h"


////////////////////// Material //////////////////////

int Material::RESERVE_PASSES_COUNT = 8;
int Material::RESERVE_PARAM_COUNT = 8;


Material::Material() :
    m_passes(),
    m_params()
{
    // Pre-allocate a chunk for passes
    if (RESERVE_PASSES_COUNT > MAX_PASSES_COUNT) {
        RESERVE_PASSES_COUNT = MAX_PASSES_COUNT;
    }

    m_passes.reserve(RESERVE_PASSES_COUNT);

    // Pre-allocate a chunk for parameters
    if (RESERVE_PARAM_COUNT > MAX_PARAM_COUNT) {
        RESERVE_PARAM_COUNT = MAX_PARAM_COUNT;
    }

    m_params.reserve(RESERVE_PARAM_COUNT);
}

RenderPass *Material::addRenderPass(const std::string &name)
{
    auto passFound = std::find_if(m_passes.begin(), m_passes.end(),
                                  [name]
                                  (const uptr<RenderPass> &pass) {
        return pass->name() == name;
    });

    auto renderPass = std::make_unique<RenderPass>(name);
    RenderPass *ret = renderPass.get();

    if (passFound == m_passes.end()) {
        m_passes.emplace_back(std::move(renderPass));
    }

    return ret;
}

void Material::removeRenderPass(RenderPass *renderPass)
{
    m_passes.erase(std::remove_if(m_passes.begin(), m_passes.end(),
                                  [renderPass]
                                  (const uptr<RenderPass> &pass) {
        return pass.get() == renderPass;
    }),
                   m_passes.end());
}

const uptr_vector<RenderPass> &Material::renderPasses() const
{
    return m_passes;
}

ShaderParam *Material::addParam(const std::string &name, const QVariant &value)
{
    auto paramFound = std::find_if(m_params.begin(), m_params.end(),
                                   [name] (const uptr<ShaderParam> &param) {
        return param->name == name;
    });

    if (paramFound != m_params.end()) {
        std::cout << "[WARNING] RenderPass : param \"" << name << "\" already exists"
                  << std::endl;
        return nullptr;
    }

    // Create the shader param and return a handle to it
    auto param = std::make_unique<ShaderParam>(name, value);
    ShaderParam *ret = param.get();

    m_params.emplace_back(std::move(param));

    return ret;
}

ShaderParam *Material::param(const std::string &name)
{
    auto paramFound = std::find_if(m_params.begin(), m_params.end(),
                                   [name] (const uptr<ShaderParam> &param) {
        return param->name == name;
    });

    if (paramFound == m_params.end()) {
        return nullptr;
    }

    return paramFound->get();
}

void Material::setParam(const std::string &name, const QVariant &value)
{
    auto paramFound = std::find_if(m_params.begin(), m_params.end(),
                                   [name] (const uptr<ShaderParam> &param) {
        return param->name == name;
    });

    if (paramFound == m_params.end()) {
        addParam(name, value);
    }
    else {
        (*paramFound)->value = value;
    }
}

void Material::removeParam(ShaderParam *param)
{
    m_params.erase(std::remove_if(m_params.begin(), m_params.end(),
                                  [param]
                                  (const uptr<ShaderParam> &p) {
        return p.get() == param;
    }),
                   m_params.end());
}

const uptr_vector<ShaderParam> &Material::params() const
{
    return m_params;
}

void Material::clearParams()
{
    m_params.clear();
}


////////////////////// Functions //////////////////////

Material defaultMaterial()
{
    Material ret;

    RenderPass *basePass = ret.addRenderPass("base");
    uptr<ShaderProgram> shader =
            shaderProgramFromFile("shaders/default.vert",
                                  "",
                                  "shaders/default.frag");

    basePass->setShaderProgram(std::move(shader));

    return ret;
}

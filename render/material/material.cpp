#include "material.h"

#include <algorithm>
#include <iostream>

#include <QDataStream>

#include "render/renderassets.h"

#include "render/material/renderpass.h"
#include "core/param.h"


////////////////////// Material //////////////////////

int Material::RESERVE_PASSES_COUNT = 8;
int Material::RESERVE_PARAM_COUNT = 8;


Material::Material() :
    m_passes(),
    m_params()
{
    init();
}

void Material::init()
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

RenderPass &Material::addRenderPass(const std::string &name)
{
    auto passFound = std::find_if(m_passes.begin(), m_passes.end(),
                                  [name]
                                  (const RenderPass &pass) {
        return pass.name() == name;
    });

    if (passFound != m_passes.end()) {
        std::cout << "[WARNING] Material: pass \"" << name << "\" already exists"
                  << std::endl;

        return *passFound;
    }

    RenderPass pass(name);

    return *m_passes.insert(m_passes.end(), pass);
}

void Material::removeRenderPass(RenderPass *renderPass)
{
    m_passes.erase(std::remove_if(m_passes.begin(), m_passes.end(),
                                  [renderPass]
                                  (const RenderPass &pass) {
        return &pass == renderPass;
    }), m_passes.end());
}

const std::vector<RenderPass> &Material::renderPasses() const
{
    return m_passes;
}

std::vector<RenderPass> &Material::renderPasses()
{
    return m_passes;
}

Param &Material::addParam(const std::string &name, const QVariant &value)
{
    auto paramFound = std::find_if(m_params.begin(), m_params.end(),
                                   [name] (const Param &param) {
        return param.name == name;
    });

    if (paramFound != m_params.end()) {
        std::cout << "[WARNING] RenderPass : param \"" << name << "\" already exists"
                  << std::endl;
        return *paramFound;
    }

    // Create the shader param and return a handle to it
    Param param {name, value};

    return *m_params.insert(m_params.end(), param);
}

Param *Material::param(const std::string &name)
{
    auto paramFound = std::find_if(m_params.begin(), m_params.end(),
                                   [name] (const Param &param) {
        return param.name == name;
    });

    if (paramFound == m_params.end()) {
        return nullptr;
    }

    return &*paramFound;
}

void Material::setParam(const std::string &name, const QVariant &value)
{
    auto paramFound = std::find_if(m_params.begin(), m_params.end(),
                                   [name] (const Param &param) {
        return param.name == name;
    });

    if (paramFound == m_params.end()) {
        addParam(name, value);
    }
    else {
        (*paramFound).value = value;
    }
}

void Material::removeParam(Param *param)
{
    m_params.erase(std::remove_if(m_params.begin(), m_params.end(),
                                  [param]
                                  (const Param &p) {
        return param->name == p.name;
    }), m_params.end());
}

const std::vector<Param> &Material::params() const
{
    return m_params;
}

std::vector<Param> &Material::params()
{
    return m_params;
}

void Material::clearParams()
{
    m_params.clear();
}


////////////////////// Functions //////////////////////

QDataStream &operator<<(QDataStream &os, const Material &material)
{
    const std::vector<RenderPass> &passes = material.renderPasses();

    os << (int) passes.size();

    for (const RenderPass &pass : passes) {
        os << pass;
    }

    const std::vector<Param> &params = material.params();

    os << (int) params.size();

    for (const Param &param : material.params()) {
        os << param;
    }
}

QDataStream &operator>>(QDataStream &os, Material &material)
{
    int passCount = 0;
    os >> passCount;

    for (int i = 0; i < passCount; i++) {
        RenderPass uglyTrick;
        os >> uglyTrick;

        RenderPass &pass = material.addRenderPass(uglyTrick.name());
        pass.setShaderProgram(uglyTrick.shaderProgram());

        for (const Param &shaderParam : uglyTrick.params()) {
            pass.addParam(shaderParam.name, shaderParam.value);
        }
    }

    int paramCount = 0;
    os >> paramCount;

    for (int i = 0; i < paramCount; i++) {
        Param param;
        os >> param;

        material.addParam(param.name, param.value);
    }
}

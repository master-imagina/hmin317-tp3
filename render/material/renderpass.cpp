#include "renderpass.h"

#include <algorithm>
#include <iostream>

#include <QDataStream>


int RenderPass::RESERVE_PARAM_COUNT = 8;


RenderPass::RenderPass() :
    m_name(),
    m_shaderProgram(),
    m_params()
{
    init();
}

RenderPass::RenderPass(const std::string &name) :
    m_name(name),
    m_shaderProgram(),
    m_params()
{
    init();
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

ShaderProgram &RenderPass::shaderProgram()
{
    return m_shaderProgram;
}

const ShaderProgram &RenderPass::shaderProgram() const
{
    return m_shaderProgram;
}

void RenderPass::setShaderProgram(const ShaderProgram &program)
{
    m_shaderProgram = program;
}

Param &RenderPass::addParam(const std::string &name, const QVariant &value)
{
    auto paramFound = std::find_if(m_params.begin(), m_params.end(),
                                  [name] (const Param &param) {
        return param.name == name;
    });

    if (paramFound != m_params.end()) {
        std::cout << "[WARNING] RenderPass: param \"" << name << "\" already exists"
                  << std::endl;
        return *paramFound;
    }

    // Create the shader param and return a handle to it
    Param param {name, value};

    return *m_params.insert(m_params.end(), param);
}

Param *RenderPass::param(const std::string &name)
{
    auto paramFound = std::find_if(m_params.begin(), m_params.end(),
                                  [name] (const Param &param) {
        return param.name == name;
    });

    if (paramFound == m_params.end()) {
        std::cerr << "RenderPass: param \"" << name << "\" not found"
                  << std::endl;
        return nullptr;
    }

    return &*paramFound;
}

void RenderPass::setParam(const std::string &name, const QVariant &value)
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

void RenderPass::removeParam(Param *param)
{
    m_params.erase(std::remove_if(m_params.begin(), m_params.end(),
                                  [param]
                                  (const Param &p) {
                        return &p == param;
    }), m_params.end());
}

const std::vector<Param> &RenderPass::params() const
{
    return m_params;
}

std::vector<Param> &RenderPass::params()
{
    return m_params;
}

void RenderPass::clearParams()
{
    m_params.clear();
}

void RenderPass::init()
{
    if (RESERVE_PARAM_COUNT > MAX_PARAM_COUNT) {
        RESERVE_PARAM_COUNT = MAX_PARAM_COUNT;
    }

    m_params.reserve(RESERVE_PARAM_COUNT);
}


////////////////////// Functions //////////////////////

QDataStream &operator<<(QDataStream &os, const RenderPass &pass)
{
    os << QString::fromStdString(pass.name());

    os << pass.shaderProgram();

    const std::vector<Param> &params = pass.params();

    os << (int) params.size();

    for (const Param &param : params) {
        os << param;
    }
}

QDataStream &operator>>(QDataStream &os, RenderPass &pass)
{
    QString passName;
    os >> passName;

    os >> pass.shaderProgram();

    int paramCount = 0;
    os >> paramCount;

    for (int i = 0; i < paramCount; i++) {
        Param param;
        os >> param;

        pass.addParam(param.name, param.value);
    }
}

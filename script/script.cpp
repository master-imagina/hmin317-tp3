#include "script.h"

#include <iostream>

#include <QDataStream>


namespace {

Param &addParamToVector(const std::string &name, const QVariant &value,
                        std::vector<Param> &params)
{
    auto paramFound = std::find_if(params.begin(), params.end(),
                                   [name] (const Param &param) {
        return param.name == name;
    });

    if (paramFound != params.end()) {
        std::cout << "[WARNING] Script : param \"" << name << "\" already exists"
                  << std::endl;
        return *paramFound;
    }

    // Create the shader param and return a handle to it
    Param param {name, value};

    return *params.insert(params.end(), param);
}

} // anon namespace


////////////////////// Script //////////////////////

Script::Script() :
    path(),
    sourceCode(),
    m_params()
{}

Script::Script(const Script &other) :
    path(other.path),
    sourceCode(other.sourceCode),
    m_params()
{
    m_params = std::vector<Param>(other.params().begin(), other.params().end());
}

Param &Script::addParam(const std::string &name, const QVariant &value)
{
    return addParamToVector(name, value, m_params);
}

Param *Script::param(const std::string &name)
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

void Script::setParam(const std::string &name, const QVariant &value)
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

void Script::removeParam(Param *param)
{
    m_params.erase(std::remove_if(m_params.begin(), m_params.end(),
                                  [param]
                                  (const Param &p) {
        return param->name == p.name;
    }), m_params.end());
}

const std::vector<Param> &Script::params() const
{
    return m_params;
}

std::vector<Param> &Script::params()
{
    return m_params;
}

const std::vector<Param> &Script::privateProperties() const
{
    return m_privateProperties;
}

void Script::setPrivateProperty(const std::string &name, const QVariant &value)
{
    auto privatePropFound = std::find_if(m_privateProperties.begin(), m_privateProperties.end(),
                                   [name] (const Param &param) {
        return param.name == name;
    });

    if (privatePropFound == m_privateProperties.end()) {
        addParamToVector(name, value, m_privateProperties);
    }
    else {
        (*privatePropFound).value = value;
    }
}


////////////////////// Functions //////////////////////

QDataStream &operator<<(QDataStream &os, const Script &script)
{
    os << QString::fromStdString(script.path);

    const std::vector<Param> &params = script.params();

    os << (int) params.size();

    // Write script properties
    for (const Param &prop : params) {
        os << prop;
    }
}

QDataStream &operator>>(QDataStream &os, Script &script)
{
    QString scriptPath;
    os >> scriptPath;

    script.path = scriptPath.toStdString();

    // Read properties
    int propCount = 0;
    os >> propCount;

    for (int i = 0; i < propCount; i++) {
        Param param;
        os >> param;

        script.addParam(param.name, param.value);
    }
}

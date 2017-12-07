#include "script.h"

#include <iostream>

#include <QDataStream>


////////////////////// Script //////////////////////

Param &Script::addParam(const std::string &name, const QVariant &value)
{
    auto paramFound = std::find_if(m_params.begin(), m_params.end(),
                                   [name] (const Param &param) {
        return param.name == name;
    });

    if (paramFound != m_params.end()) {
        std::cout << "[WARNING] Script : param \"" << name << "\" already exists"
                  << std::endl;
        return *paramFound;
    }

    // Create the shader param and return a handle to it
    Param param {name, value};

    return *m_params.insert(m_params.end(), param);
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

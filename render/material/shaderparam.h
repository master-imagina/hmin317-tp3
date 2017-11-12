#ifndef SHADERPARAM_H
#define SHADERPARAM_H

#include <QVariant>


struct ShaderParam
{
    ShaderParam();
    ShaderParam(const std::string &_name, const QVariant &_value);
    ~ShaderParam() = default;

    std::string name;
    QVariant value;
};

#endif // SHADERPARAM_H

#ifndef SHADERPARAM_H
#define SHADERPARAM_H

#include <string>

#include <QVariant>


struct ShaderParam
{
    QByteArray name;
    QVariant value;
};

bool operator==(const ShaderParam &lhs, const ShaderParam &rhs);

#endif // SHADERPARAM_H

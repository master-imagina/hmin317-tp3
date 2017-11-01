#ifndef SHADERPARAM_H
#define SHADERPARAM_H

#include <QVariant>


struct ShaderParam
{
    ShaderParam();
    ShaderParam(const QByteArray &_name, const QVariant &_value);

    QByteArray name;
    QVariant value;
};

#endif // SHADERPARAM_H

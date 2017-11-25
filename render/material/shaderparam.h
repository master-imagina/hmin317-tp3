#ifndef SHADERPARAM_H
#define SHADERPARAM_H

#include <QVariant>

class QDataStream;


struct ShaderParam
{
    ShaderParam();
    ShaderParam(const std::string &_name, const QVariant &_value);
    ~ShaderParam() = default;

    std::string name;
    QVariant value;
};


QDataStream &operator<<(QDataStream &os, const ShaderParam &param);
QDataStream &operator>>(QDataStream &os, ShaderParam &param);

#endif // SHADERPARAM_H

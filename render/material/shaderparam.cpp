#include "shaderparam.h"


ShaderParam::ShaderParam() :
    name(),
    value()
{}

ShaderParam::ShaderParam(const std::string &_name, const QVariant &_value) :
    name(_name),
    value(_value)
{}


////////////////////// Functions //////////////////////

QDataStream &operator<<(QDataStream &os, const ShaderParam &param)
{
    os << QString::fromStdString(param.name);
    os << param.value;
}

QDataStream &operator>>(QDataStream &os, ShaderParam &param)
{
    QString paramName;
    os >> paramName;
    param.name = paramName.toStdString();

    os >> param.value;
}

#include "shaderparam.h"


ShaderParam::ShaderParam() :
    name(),
    value()
{
    ;
}

ShaderParam::ShaderParam(const QByteArray &_name, const QVariant &_value) :
    name(_name),
    value(_value)
{}

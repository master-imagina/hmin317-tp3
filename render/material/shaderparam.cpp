#include "shaderparam.h"


ShaderParam::ShaderParam() :
    name(),
    value()
{}

ShaderParam::ShaderParam(const std::string &_name, const QVariant &_value) :
    name(_name),
    value(_value)
{}

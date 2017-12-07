#include "param.h"


Param::Param() :
    name(),
    value()
{}

Param::Param(const std::string &_name, const QVariant &_value) :
    name(_name),
    value(_value)
{}


////////////////////// Functions //////////////////////

QDataStream &operator<<(QDataStream &os, const Param &param)
{
    os << QString::fromStdString(param.name);
    os << param.value;
}

QDataStream &operator>>(QDataStream &os, Param &param)
{
    QString paramName;
    os >> paramName;
    param.name = paramName.toStdString();

    os >> param.value;
}

#ifndef PARAM_H
#define PARAM_H

#include <QVariant>

class QDataStream;


struct Param
{
    Param();
    Param(const std::string &_name, const QVariant &_value);
    ~Param() = default;

    std::string name;
    QVariant value;
};


QDataStream &operator<<(QDataStream &os, const Param &param);
QDataStream &operator>>(QDataStream &os, Param &param);

#endif // PARAM_H

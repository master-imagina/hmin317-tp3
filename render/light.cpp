#include "light.h"



QDataStream &operator<<(QDataStream &os, const Light &light)
{
    os << light.pos
       << light.color;
}

QDataStream &operator>>(QDataStream &os, Light &light)
{
    os >> light.pos;
    os >> light.color;
}

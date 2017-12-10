#include "light.h"


QDataStream &operator<<(QDataStream &os, const Light &light)
{
    os << light.pos
       << light.direction
       << light.color;
}

QDataStream &operator>>(QDataStream &os, Light &light)
{
    os >> light.pos;
    os >> light.direction;
    os >> light.color;
}

#ifndef LIGHT_H
#define LIGHT_H

#include <QColor>
#include <QDataStream>
#include <QVector3D>


struct Light
{
    QVector3D pos;
    QVector3D direction;
    QColor color;
};


QDataStream &operator<<(QDataStream &os, const Light &light);
QDataStream &operator>>(QDataStream &os, Light &light);

#endif // LIGHT_H

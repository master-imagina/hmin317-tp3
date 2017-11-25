#ifndef LIGHT_H
#define LIGHT_H

#include <QColor>
#include <QVector3D>


struct Light
{
    QVector3D pos;
    QColor color;
};

#endif // LIGHT_H

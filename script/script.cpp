#include "script.h"

#include <QDataStream>


QDataStream &operator<<(QDataStream &os, const Script &script)
{
    os << QString::fromStdString(script.path);
}

QDataStream &operator>>(QDataStream &os, Script &script)
{
    QString scriptPath;
    os >> scriptPath;

    script.path = scriptPath.toStdString();
}

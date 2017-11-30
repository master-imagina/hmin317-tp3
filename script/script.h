#ifndef SCRIPT_H
#define SCRIPT_H

#include <QString>

class QDataStream;


struct Script
{
    std::string path;
    QByteArray sourceCode;
};


QDataStream &operator<<(QDataStream &os, const Script &script);
QDataStream &operator>>(QDataStream &os, Script &script);

#endif // SCRIPT_H

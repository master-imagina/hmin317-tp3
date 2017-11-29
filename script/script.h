#ifndef SCRIPT_H
#define SCRIPT_H

#include <QString>


struct Script
{
    std::string path;
    QByteArray sourceCode;
};

#endif // SCRIPT_H

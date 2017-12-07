#ifndef SCRIPT_H
#define SCRIPT_H

#include <unordered_map>

#include <QByteArray>
#include <QVariant>

#include "core/param.h"

class QDataStream;


struct Script
{
    std::string path;
    QByteArray sourceCode;

    Param &addParam(const std::string &name, const QVariant &value);
    Param *param(const std::string &name);
    void setParam(const std::string &name, const QVariant &value);
    void removeParam(Param *param);

    const std::vector<Param> &params() const;
    std::vector<Param> &params();

private:
    std::vector<Param> m_params;
};


QDataStream &operator<<(QDataStream &os, const Script &script);
QDataStream &operator>>(QDataStream &os, Script &script);

#endif // SCRIPT_H

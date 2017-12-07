#ifndef RENDERPASS_H
#define RENDERPASS_H

#include <string>
#include <vector>

#include <QVariant>

#include "core/param.h"

#include "shaderprogram.h"

class QDataStream;


class RenderPass
{
public:
    static int RESERVE_PARAM_COUNT;
    static const int MAX_PARAM_COUNT = 1024;

    RenderPass();
    explicit RenderPass(const std::string &name);
    ~RenderPass();

    std::string name() const;
    void setName(const std::string &name);

    ShaderProgram &shaderProgram();
    const ShaderProgram &shaderProgram() const;
    void setShaderProgram(const ShaderProgram &program);

    Param &addParam(const std::string &name, const QVariant &value);
    Param *param(const std::string &name);
    void setParam(const std::string &name, const QVariant &value);
    void removeParam(Param *param);

    const std::vector<Param> &params() const;
    std::vector<Param> &params();

    void clearParams();

private:
    void init();

private:
    std::string m_name;
    ShaderProgram m_shaderProgram;
    std::vector<Param> m_params;
};


QDataStream &operator<<(QDataStream &os, const RenderPass &pass);
QDataStream &operator>>(QDataStream &os, RenderPass &pass);

#endif // RENDERPASS_H

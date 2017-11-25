#ifndef RENDERPASS_H
#define RENDERPASS_H

#include <string>
#include <vector>

#include <QVariant>

#include "shaderparam.h"
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

    ShaderParam &addParam(const std::string &name, const QVariant &value);
    ShaderParam *param(const std::string &name);
    void setParam(const std::string &name, const QVariant &value);
    void removeParam(ShaderParam *param);

    const std::vector<ShaderParam> &params() const;
    std::vector<ShaderParam> &params();

    void clearParams();

private:
    void init();

private:
    std::string m_name;
    ShaderProgram m_shaderProgram;
    std::vector<ShaderParam> m_params;
};


QDataStream &operator<<(QDataStream &os, const RenderPass &pass);
QDataStream &operator>>(QDataStream &os, RenderPass &pass);

#endif // RENDERPASS_H

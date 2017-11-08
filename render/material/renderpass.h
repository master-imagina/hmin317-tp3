#ifndef RENDERPASS_H
#define RENDERPASS_H

#include <string>
#include <vector>

#include "core/aliases_memory.h"

#include "shaderparam.h"

class ShaderProgram;


class RenderPass
{
public:
    static int RESERVE_PARAM_COUNT;
    static const int MAX_PARAM_COUNT = 1024;

    explicit RenderPass(const std::string &name);
    ~RenderPass();

    std::string name() const;
    void setName(const std::string &name);

    ShaderProgram *shaderProgram() const;
    void setShaderProgram(uptr<ShaderProgram> &&program);

    ShaderParam *addParam(const std::string &name, const QVariant &value);
    ShaderParam *param(const std::string &name);
    void setParam(const std::string &name, const QVariant &value);
    void removeParam(ShaderParam *param);

    const uptr_vector<ShaderParam> &params() const;

    void clearParams();

private:
    std::string m_name;
    uptr<ShaderProgram> m_shaderProgram;
    uptr_vector<ShaderParam> m_params;
};

#endif // RENDERPASS_H

#ifndef RENDERPASS_H
#define RENDERPASS_H

#include <string>
#include <vector>

#include "aliases_memory.h"
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

    void addParam(const ShaderParam &param);
    void removeParam(const ShaderParam &param);

    std::vector<ShaderParam> params() const;

    void clearParams();

private:
    std::string m_name;
    uptr<ShaderProgram> m_shaderProgram;
    std::vector<ShaderParam> m_params;
};

#endif // RENDERPASS_H

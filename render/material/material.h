#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include <string>

#include <QVariant>

#include "core/aliases_memory.h"

class RenderPass;
class ShaderParam;


class Material
{
public:
    static int RESERVE_PARAM_COUNT;
    static const int MAX_PARAM_COUNT = 1024;

    static int RESERVE_PASSES_COUNT;
    static const int MAX_PASSES_COUNT = 16;

    Material();

    RenderPass *addRenderPass(const std::string &name);
    void removeRenderPass(RenderPass *renderPass);

    const uptr_vector<RenderPass> &renderPasses() const;

    ShaderParam *addParam(const std::string &name, const QVariant &value);
    ShaderParam *param(const std::string &name);
    void setParam(const std::string &name, const QVariant &value);
    void removeParam(ShaderParam *param);

    const uptr_vector<ShaderParam> &params() const;

    void clearParams();

private:
    uptr_vector<RenderPass> m_passes;
    uptr_vector<ShaderParam> m_params;
};


Material defaultMaterial();

#endif // MATERIAL_H

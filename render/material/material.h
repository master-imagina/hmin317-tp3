#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include <string>

#include <QVariant>

class QDataStream;

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

    RenderPass &addRenderPass(const std::string &name);
    void removeRenderPass(RenderPass *renderPass);

    const std::vector<RenderPass> &renderPasses() const;
    std::vector<RenderPass> &renderPasses();

    ShaderParam &addParam(const std::string &name, const QVariant &value);
    ShaderParam *param(const std::string &name);
    void setParam(const std::string &name, const QVariant &value);
    void removeParam(ShaderParam *param);

    const std::vector<ShaderParam> &params() const;
    std::vector<ShaderParam> &params();

    void clearParams();

private:
    std::vector<RenderPass> m_passes;
    std::vector<ShaderParam> m_params;
};


QDataStream &operator<<(QDataStream &os, const Material &material);
QDataStream &operator>>(QDataStream &os, Material &material);

#endif // MATERIAL_H

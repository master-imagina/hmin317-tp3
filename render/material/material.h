#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include <string>

#include <QVariant>

class QDataStream;

class RenderPass;
class Param;


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
    std::vector<RenderPass> m_passes;
    std::vector<Param> m_params;
};


QDataStream &operator<<(QDataStream &os, const Material &material);
QDataStream &operator>>(QDataStream &os, Material &material);

#endif // MATERIAL_H

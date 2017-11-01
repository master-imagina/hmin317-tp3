#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include <string>
#include <vector>

#include "../aliases_memory.h"

class RenderPass;


class Material
{
public:
    static int RESERVE_PASSES_COUNT;
    static const int MAX_PASSES_COUNT = 16;

    Material();

    RenderPass *addRenderPass(const std::string &name);
    void removeRenderPass(RenderPass *renderPass);

    const uptr_vector<RenderPass> &renderPasses() const;

private:
    std::vector<uptr<RenderPass>> m_passes;
};

#endif // MATERIAL_H

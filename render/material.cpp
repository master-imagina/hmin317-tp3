#include "material.h"

#include <algorithm>

#include "renderpass.h"


int Material::RESERVE_PASSES_COUNT = 8;


Material::Material() :
    m_passes()
{
    if (RESERVE_PASSES_COUNT > MAX_PASSES_COUNT) {
        RESERVE_PASSES_COUNT = MAX_PASSES_COUNT;
    }

    m_passes.reserve(RESERVE_PASSES_COUNT);
}

RenderPass *Material::addRenderPass(const std::string &name)
{
    auto passFound = std::find_if(m_passes.begin(), m_passes.end(),
                                  [name]
                                  (const uptr<RenderPass> &pass) {
        return pass->name() == name;
    });

    auto renderPass = std::make_unique<RenderPass>(name);
    RenderPass *ret = renderPass.get();

    if (passFound == m_passes.end()) {
        m_passes.emplace_back(std::move(renderPass));
    }

    return ret;
}

void Material::removeRenderPass(RenderPass *renderPass)
{
    m_passes.erase(std::remove_if(m_passes.begin(), m_passes.end(),
                                  [renderPass]
                                  (const uptr<RenderPass> &pass) {
                        return pass.get() == renderPass;
                   }),
                   m_passes.end());
}

const uptr_vector<RenderPass> &Material::renderPasses() const
{
    return m_passes;
}

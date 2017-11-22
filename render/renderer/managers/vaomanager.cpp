#include "vaomanager.h"

#include <iostream>

#include "render/geometry/geometry.h"

#include "render/renderer/glvao.h"
#include "render/renderer/glwrapper.h"


int VaoManager::RESERVE_VAO_COUNT = 64;

VaoManager::VaoManager() :
    m_vaos(),
    m_geomToVao()
{
    m_geomToVao.reserve(RESERVE_VAO_COUNT);
}

GLVao *VaoManager::addGeometry(Geometry *geom, GLWrapper &glWrapper)
{
    const auto isHandled = m_geomToVao.find(geom);

    if (isHandled != m_geomToVao.cend()) {
        std::cout << "[WARNING] VaoManager::addGeometry(): vao already exists" << std::endl;
        return nullptr;
    }

    auto glVao = std::make_unique<GLVao>();
    GLVao *ret = glVao.get();

    // Create GL VAO
    glWrapper.createVao(*ret);

    m_vaos.emplace_back(std::move(glVao));

    m_geomToVao.insert({geom, ret});

    return ret;
}

bool VaoManager::isAllocated(Geometry *geom) const
{
    const GLVao *glVao = get(geom);

    return glVao != nullptr;
}

GLVao *VaoManager::get(Geometry *geom) const
{
    const auto geomFound = m_geomToVao.find(geom);

    GLVao *ret = nullptr;

    if (geomFound != m_geomToVao.end()) {
        ret = geomFound->second;
    }

    return ret;
}

void VaoManager::cleanup(GLWrapper &glWrapper)
{
    for (auto &vao : m_vaos) {
        glWrapper.destroyVao(*vao.get());
    }
}

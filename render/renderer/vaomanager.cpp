#include "vaomanager.h"

#include <assert.h>
#include <iostream>

#include "../geometry/geometry.h"

#include "glwrapper.h"


int VaoManager::RESERVE_VAO_COUNT = 64;

VaoManager::VaoManager() :
    m_geomToVao()
{
    m_geomToVao.reserve(RESERVE_VAO_COUNT);
}

uint32 VaoManager::addGeometry(Geometry *geom, GLWrapper &glWrapper)
{
    const auto isHandled = m_geomToVao.find(geom);

    if (isHandled != m_geomToVao.cend()) {
        std::cerr << "VaoManager::addGeometry(): vao already exists" << std::endl;
        return 0;
    }

    uint32 ret = glWrapper.createVao();

    m_geomToVao.insert({geom, ret});

    return ret;
}

bool VaoManager::isAllocated(Geometry *geom) const
{
    const auto geomFound = m_geomToVao.find(geom);

    if (geomFound != m_geomToVao.end()) {
        return true;
    }

    return false;
}

uint32 VaoManager::vaoForGeometry(Geometry *geom) const
{
    const auto geomFound = m_geomToVao.find(geom);

    uint32 ret = 0;

    if (geomFound != m_geomToVao.end()) {
        ret = geomFound->second;
    }

    return ret;
}

void VaoManager::cleanup(GLWrapper &glWrapper)
{
    for (auto &geomAndVao : m_geomToVao) {
        glWrapper.destroyVao(geomAndVao.second);
    }
}

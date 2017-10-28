#include "buffermanager.h"

#include <QDebug>

#include "../geometry.h"


BufferManager::BufferManager() :
    m_geomToBuffers()
{}

void BufferManager::addGeometry(Geometry *geom,
                                GLBuffer *vertexBuffer,
                                GLBuffer *indexBuffer)
{
    const auto isHandled = m_geomToBuffers.find(geom);

    if (isHandled == m_geomToBuffers.cend()) {
        m_geomToBuffers.insert({ geom, {vertexBuffer, indexBuffer} });
    }
}

std::pair<GLBuffer *, GLBuffer *> BufferManager::buffersForGeometry(Geometry *geom)
{
    const auto geomFound = m_geomToBuffers.find(geom);

    std::pair<GLBuffer *, GLBuffer *> ret {nullptr, nullptr};

    if (geomFound != m_geomToBuffers.end()) {
        ret = geomFound->second;
    }

    return ret;
}

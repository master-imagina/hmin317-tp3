#include "buffermanager.h"

#include <assert.h>
#include <iostream>

#include "render/geometry/geometry.h"

#include "render/renderer/glbuffer.h"
#include "render/renderer/glwrapper.h"


int BufferManager::RESERVE_BUFFERS_COUNT = 64;

BufferManager::BufferManager() :
    m_buffers(),
    m_geomToBuffers()
{
    m_buffers.reserve(RESERVE_BUFFERS_COUNT);
}

std::pair<GLBuffer *, GLBuffer *> BufferManager::addGeometry(Geometry *geom, GLWrapper &glWrapper)
{
    const auto isHandled = m_geomToBuffers.find(geom);

    if (isHandled != m_geomToBuffers.cend()) {
        std::cout << "[WARNING] BufferManager::addGeometry(): buffers already exist" << std::endl;
        return {};
    }

    const GLBuffer::Usage bufferUsage = geom->isDynamic ? GLBuffer::Usage::StreamDraw
                                                        : GLBuffer::Usage::StaticDraw;

    uptr<GLBuffer> arrayBuffer = std::make_unique<GLBuffer>();

    glWrapper.createBuffer(*arrayBuffer.get(),
                           GLBuffer::Type::ArrayBuffer,
                           bufferUsage);

    uptr<GLBuffer> indexBuffer;

    if (geom->isIndexed || geom->indices.size() > 0) { // The hacky way
        indexBuffer = std::make_unique<GLBuffer>();

        glWrapper.createBuffer(*indexBuffer.get(),
                               GLBuffer::Type::IndexBuffer,
                               bufferUsage);
    }

    std::pair<GLBuffer *, GLBuffer *> ret {arrayBuffer.get(), indexBuffer.get()};

    m_buffers.emplace_back(std::move(arrayBuffer));

    if (indexBuffer) {
        m_buffers.emplace_back(std::move(indexBuffer));
    }

    m_geomToBuffers.insert({ geom, ret });

    return ret;
}

bool BufferManager::isAllocated(Geometry *geom) const
{
    const auto geomFound = m_geomToBuffers.find(geom);

    if (geomFound != m_geomToBuffers.end()) {
        return true;
    }

    return false;
}

std::pair<GLBuffer *, GLBuffer *> BufferManager::buffersForGeometry(Geometry *geom) const
{
    const auto geomFound = m_geomToBuffers.find(geom);

    std::pair<GLBuffer *, GLBuffer *> ret {nullptr, nullptr};

    if (geomFound != m_geomToBuffers.end()) {
        ret = geomFound->second;
    }

    return ret;
}

void BufferManager::cleanup(GLWrapper &glWrapper)
{
    for (uptr<GLBuffer> &gpuBuffer : m_buffers) {
        assert (gpuBuffer);

        glWrapper.destroyBuffer(*gpuBuffer.get());
    }
}

#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H

#include <unordered_map>
#include <vector>

#include "core/aliases_memory.h"

class Geometry;
class GLBuffer;
class GLWrapper;


class BufferManager
{
public:
    static int RESERVE_BUFFERS_COUNT;

    BufferManager();

    std::pair<GLBuffer *, GLBuffer *> addGeometry(Geometry *geom, GLWrapper &glWrapper);
    bool isAllocated(Geometry *geom) const;

    std::pair<GLBuffer *, GLBuffer *> buffersForGeometry(Geometry *geom) const;

    void cleanup(GLWrapper &glWrapper);

private:
    uptr_vector<GLBuffer> m_buffers;
    std::unordered_map<Geometry *, std::pair<GLBuffer *, GLBuffer *>> m_geomToBuffers;
};

#endif // BUFFERMANAGER_H

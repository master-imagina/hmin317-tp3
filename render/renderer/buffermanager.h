#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H

#include <map>
#include <vector>

#include "core/aliases_memory.h"

#include "glbuffer.h"

class Geometry;
class GLWrapper;


class BufferManager
{
public:
    static int RESERVE_BUFFERS_COUNT;

    BufferManager();

    std::pair<GLBuffer *, GLBuffer *> addGeometry(Geometry *geom, GLWrapper &glWrapper);

    std::pair<GLBuffer *, GLBuffer *> buffersForGeometry(Geometry *geom) const;

    void cleanup(GLWrapper &glWrapper);

private:
    uptr_vector<GLBuffer> m_buffers;
    std::map<Geometry *, std::pair<GLBuffer *, GLBuffer *>> m_geomToBuffers;
};

#endif // BUFFERMANAGER_H

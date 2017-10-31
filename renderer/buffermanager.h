#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H

#include <map>

#include "glbuffer.h"

class Geometry;


class BufferManager
{
public:
    BufferManager();

    void addGeometry(Geometry *geom,
                     GLBuffer *vertexBuffer, GLBuffer *indexBuffer);

    std::pair<GLBuffer *, GLBuffer *> buffersForGeometry(Geometry *geom);

private:
    std::map<Geometry *, std::pair<GLBuffer *, GLBuffer *>> m_geomToBuffers;
};

#endif // BUFFERMANAGER_H

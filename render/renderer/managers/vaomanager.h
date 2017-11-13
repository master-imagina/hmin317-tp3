#ifndef VAOMANAGER_H
#define VAOMANAGER_H

#include <unordered_map>
#include <vector>

#include "core/aliases_memory.h"

#include "../glbuffer.h"

class Geometry;
class GLWrapper;


class VaoManager
{
public:
    static int RESERVE_VAO_COUNT;

    VaoManager();

    uint32 addGeometry(Geometry *geom, GLWrapper &glWrapper);
    bool isAllocated(Geometry *geom) const;

    uint32 vaoForGeometry(Geometry *geom) const;

    void cleanup(GLWrapper &glWrapper);

private:
    std::unordered_map<Geometry *, uint32> m_geomToVao;
};

#endif // VAOMANAGER_H

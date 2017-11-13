#ifndef VAOMANAGER_H
#define VAOMANAGER_H

#include <unordered_map>

#include "core/aliases_memory.h"

class Geometry;
class GLVao;
class GLWrapper;


class VaoManager
{
public:
    static int RESERVE_VAO_COUNT;

    VaoManager();

    GLVao *addGeometry(Geometry *geom, GLWrapper &glWrapper);
    bool isAllocated(Geometry *geom) const;

    GLVao *get(Geometry *geom) const;

    void cleanup(GLWrapper &glWrapper);

private:
    uptr_vector<GLVao> m_vaos;
    std::unordered_map<Geometry *, GLVao *> m_geomToVao;
};

#endif // VAOMANAGER_H

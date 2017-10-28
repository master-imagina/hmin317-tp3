#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "aabb.h"
#include "geometry.h"


struct Scene
{
    std::vector<Geometry *> geometries;
    AABoundingBox terrainBoundingBox;
};

#endif // SCENE_H

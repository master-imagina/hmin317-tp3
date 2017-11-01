#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "aabb.h"
#include "geometry.h"
#include "material.h"


struct Scene
{
    std::vector<Geometry *> geometries;
    std::vector<Material *> materials;

    AABoundingBox terrainBoundingBox;
};

#endif // SCENE_H

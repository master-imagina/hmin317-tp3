#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "geometry/geometry.h"

#include "material/material.h"

#include "aabb.h"


struct Scene
{
    std::vector<Geometry *> geometries;
    std::vector<Material *> materials;

    AABoundingBox terrainBoundingBox;
};

#endif // SCENE_H

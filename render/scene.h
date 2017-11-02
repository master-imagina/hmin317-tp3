#ifndef SCENE_H
#define SCENE_H

#include <functional>
#include <vector>

#include "aabb.h"

class Geometry;
class Material;


class Scene
{
    using RenderableAddedCallback = std::function<void (Geometry *, Material *)>;

public:
    Scene();
    ~Scene();

    void addRenderable(Geometry *geom, Material *material);

    void connectRenderableAdded(const RenderableAddedCallback &callback);

    const std::vector<Geometry *> &geometries() const;
    const std::vector<Material *> &materials() const;

public:
    AABoundingBox terrainBoundingBox;

private:
    std::vector<Geometry *> m_geometries;
    std::vector<Material *> m_materials;

    std::vector<RenderableAddedCallback> m_renderableAddedCallbacks;
};

#endif // SCENE_H

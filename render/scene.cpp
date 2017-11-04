#include "scene.h"

#include "geometry/geometry.h"

#include "material/material.h"


Scene::Scene() :
    m_geometries(),
    m_materials(),
    m_renderableAddedCallbacks()
{}

Scene::~Scene()
{}

void Scene::addRenderable(Geometry *geom, Material *material)
{
    m_geometries.emplace_back(geom);
    m_materials.emplace_back(material);

    for (RenderableAddedCallback &callback : m_renderableAddedCallbacks) {
        callback(geom, material);
    }
}

void Scene::connectRenderableAdded(const Scene::RenderableAddedCallback &callback)
{
    m_renderableAddedCallbacks.emplace_back(callback);
}

const std::vector<Geometry *> &Scene::geometries() const
{
    return m_geometries;
}

const std::vector<Material *> &Scene::materials() const
{
    return m_materials;
}

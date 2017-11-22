#include "rendersystem.h"

#include "render/geometry/geometry.h"

#include "render/material/material.h"

#include "render/renderer/renderer.h"

#include "render/mesh.h"
#include "render/renderwidget.h"
#include "render/transform.h"


RenderSystem::RenderSystem(RenderWidget *surface) :
    m_surface(surface),
    m_renderer(surface->renderer())
{}

void RenderSystem::update(entityx::EntityManager &entities,
                          entityx::EventManager &events,
                          double dt)
{
//    m_surface->makeCurrent();

    entities.each<Geometry, Material, Transform>(
                [this] (entityx::Entity entity,
                        Geometry &geom, Material &mat, Transform &transform) {
        m_renderer->prepareDrawCommand(geom, mat, transform);
    });

    entities.each<Mesh, Material, Transform>(
                [this] (entityx::Entity entity,
                        Mesh &mesh, Material &mat, Transform &transform) {
        for (Geometry &geom : mesh.geometries()) {
            m_renderer->prepareDrawCommand(geom, mat, transform);
        }
    });

//    m_surface->doneCurrent();


    m_surface->startNewFrame(dt);
}

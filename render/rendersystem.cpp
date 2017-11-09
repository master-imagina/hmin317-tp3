#include "rendersystem.h"

#include "geometry/geometry.h"
#include "material/material.h"
#include "renderer/renderer.h"

#include "transform.h"

#include "editor/renderwidget.h" //FIXME


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
                        Geometry &geom, Material &mat, Transform &tranform) {
        m_renderer->prepareDrawCommand(entity);
    });

//    m_surface->doneCurrent();


    m_surface->startNewFrame(dt);
}

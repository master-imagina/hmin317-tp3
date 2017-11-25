#include "rendersystem.h"

#include "render/geometry/geometry.h"

#include "render/material/material.h"
#include "render/material/renderpass.h"
#include "render/material/shaderparam.h"

#include "render/renderer/renderer.h"

#include "render/light.h"
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
    // Gather lights
    entities.each<Light, Transform>(
                [this] (entityx::Entity entity, Light &light, Transform &transform) {
        light.pos = transform.translate();

        m_renderer->addLight(light);
    });

    // Prepare draw commands
    entities.each<Geometry, Transform>(
                [this] (entityx::Entity entity,
                        Geometry &geom, Transform &transform) {
        //TODO optimize that, why not having a component dependency from
        //  Geometry to Material ?
        if (!entity.has_component<Material>()) {
            entity.assign<Material>(defaultMaterial());
        }

        m_renderer->prepareDrawCommand(geom,
                                       *entity.component<Material>().get(),
                                       transform);
    });

    entities.each<Mesh, Transform>(
                [this] (entityx::Entity entity,
                        Mesh &mesh, Transform &transform) {
        for (int i = 0;i < mesh.count(); i++) {
            if (!entity.has_component<Material>()) {
                entity.assign<Material>(defaultMaterial());
            }

            m_renderer->prepareDrawCommand(mesh.geometry(i),
                                           *entity.component<Material>().get(),
                                           transform);
        }
    });

    // Render
    m_surface->startNewFrame(dt);
}

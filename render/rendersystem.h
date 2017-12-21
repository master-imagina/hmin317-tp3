#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <entityx/System.h>

class Renderer;
class RenderWidget;


class RenderSystem : public entityx::System<RenderSystem>
{
public:
    explicit RenderSystem(RenderWidget *surface);

    void configure(entityx::EntityManager &entities,
                   entityx::EventManager &events) override;

    void update(entityx::EntityManager &entities,
                entityx::EventManager &events,
                double dt) override;

private:
    RenderWidget *m_surface;
    Renderer *m_renderer;
};

#endif // RENDERSYSTEM_H

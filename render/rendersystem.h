#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "3rdparty/entityx/System.h"

class Renderer;
class RenderWidget;


class RenderSystem : public entityx::System<RenderSystem>
{
public:
    explicit RenderSystem(RenderWidget *surface);

    void update(entityx::EntityManager &entities,
                entityx::EventManager &events,
                double dt) override;

private:
    RenderWidget *m_surface;
    Renderer *m_renderer;
};

#endif // RENDERSYSTEM_H

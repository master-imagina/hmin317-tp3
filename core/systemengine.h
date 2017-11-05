#ifndef SYSTEMENGINE_H
#define SYSTEMENGINE_H

#include "3rdparty/entityx/System.h"

class Scene;


class SystemEngine
{
public:
    SystemEngine(Scene &scene);
    ~SystemEngine() = default;

    template <class T, typename ... Args>
    void registerSystem(Args && ... args)
    {
        m_systemManager.add<T>(std::forward<Args>(args) ...);
    }

    template <class T>
    void update(float dt)
    {
        m_systemManager.update<T>(dt);
    }

    void initialize();

private:
    Scene &m_scene;
    entityx::SystemManager m_systemManager;
};


#endif // SYSTEMENGINE_H

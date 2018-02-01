#ifndef SYSTEMENGINE_H
#define SYSTEMENGINE_H

#include <entityx/System.h>
#include <entityx/deps/Dependencies.h>

class Scene;


class SystemEngine
{
public:
    SystemEngine(Scene &scene);
    ~SystemEngine() = default;

    template <class T, typename ... Args>
    std::shared_ptr<T> registerSystem(Args && ... args)
    {
        return m_systemManager.add<T>(std::forward<Args>(args) ...);
    }

    template <class T>
    std::shared_ptr<T> system()
    {
        return m_systemManager.system<T>();
    }

    template <class T, typename ... Deps>
    void registerDependency()
    {
        registerSystem<entityx::deps::Dependency<T, Deps...>>();
    }

    template <class T>
    void update(float dt)
    {
        m_systemManager.update<T>(dt);
    }

    void configure();

private:
    Scene &m_scene;
    entityx::SystemManager m_systemManager;
};

#endif // SYSTEMENGINE_H

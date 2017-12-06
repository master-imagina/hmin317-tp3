#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <functional>

#include <QObject>

#include "core/gameloop.h"
#include "core/systemengine.h"

#include "render/renderwidget.h"

class LuaServer;


class GameWidget : public RenderWidget
{
public:
    GameWidget(Scene &scene, LuaServer *luaServer = nullptr,
               QWidget *parent = nullptr);

    GameLoop *gameLoop() const;

    SystemEngine &systemEngine();

    void enableScripts();
    void disableScripts();

protected:
    void showEvent(QShowEvent *) override;

private:
    void initSystems();
    void iterateGameLoop(float dt);

private:
    bool m_initialized;

    SystemEngine m_systemEngine;

    LuaServer *m_theLuaServer;
    GameLoop *m_gameLoop;

    bool m_scriptsEnabled;
};

#endif // GAMEWIDGET_H

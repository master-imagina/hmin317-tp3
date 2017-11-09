#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <functional>

#include <QObject>

#include "core/gameloop.h"
#include "core/systemengine.h"

#include "editor/renderwidget.h"

class CameraController;
class Scene;


class GameWidget : public RenderWidget
{
public:
    explicit GameWidget(Scene &scene, QWidget *parent = nullptr);

    GameLoop *gameLoop() const;

private:
    void initSystems();
    void iterateGameLoop(float dt);

private:
    SystemEngine m_systemEngine;

    GameLoop *m_gameLoop;

    CameraController *m_cameraController;
};

#endif // GAMEWIDGET_H

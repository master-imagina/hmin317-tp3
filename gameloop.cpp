#include "gameloop.h"


GameLoop::GameLoop(unsigned int fps, QObject *parent) :
    QObject(parent),
    m_fps(fps),
    m_timer(),
    m_deltaTime(),
    m_callback()
{
}

unsigned int GameLoop::fps() const
{
    return m_fps;
}

void GameLoop::setFps(unsigned int fps)
{
    if (m_fps != fps) {
        m_fps = fps;

        run();
    }
}

void GameLoop::setCallback(const std::function<void (float)> &callback)
{
    m_callback = callback;
}

void GameLoop::run()
{
    if (m_fps != 0) {
        m_deltaTime.start();

        m_timer.start(1000 / m_fps, this);
    }
    else {
        m_timer.stop();
    }
}

void GameLoop::timerEvent(QTimerEvent *)
{
    const int dt = m_deltaTime.elapsed();

    m_deltaTime.restart();

    if (m_callback) {
        m_callback(dt);
    }
}

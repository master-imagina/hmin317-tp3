#include "gameloop.h"


GameLoop::GameLoop(unsigned int fps, QObject *parent) :
    QObject(parent),
    m_fps(fps),
    m_timer(),
    m_deltaTime(),
    m_callback()
{
    m_deltaTime.start();
}

unsigned int GameLoop::fps() const
{
    return m_fps;
}

void GameLoop::setCallback(const std::function<void (float)> &callback)
{
    m_callback = callback;
}

void GameLoop::run()
{
    m_timer.start(1000 / m_fps, this);
}

void GameLoop::timerEvent(QTimerEvent *)
{
    const int dt = m_deltaTime.elapsed();

    m_deltaTime.restart();

    if (m_callback) {
        m_callback(dt);
    }
}

#include "gameloop.h"


GameLoop::GameLoop(uint32 fps, QObject *parent) :
    QObject(parent),
    m_fps(fps),
    m_timer(),
    m_deltaTime(),
    m_lastDeltas(),
    m_currentDeltaIndex(),
    m_callback()
{}

uint32 GameLoop::framerate() const
{
    return m_fps;
}

void GameLoop::setFramerate(uint32 fps)
{
    if (m_fps != fps) {
        m_fps = fps;

        run();
    }
}

uint32 GameLoop::effectiveFramerate() const
{
    float fps = 0;

    for (int i = 0; i < 10; ++i) {
        fps += m_lastDeltas[i];
    }

    return 1000.0 / (fps / 10.0);
}

void GameLoop::setCallback(const std::function<void (float)> &callback)
{
    m_callback = callback;
}

void GameLoop::run()
{
    if (m_fps != 0) {
        m_deltaTime.start();

        m_lastDeltas.fill(10);
        m_currentDeltaIndex = 0;

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

    m_lastDeltas[m_currentDeltaIndex++] = dt;

    m_currentDeltaIndex = m_currentDeltaIndex % 10;

    if (m_callback) {
        m_callback(dt);
    }
}

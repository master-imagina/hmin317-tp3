#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <functional>

#include <QBasicTimer>
#include <QObject>
#include <QTime>

#include "aliases_int.h"


class GameLoop : public QObject
{
public:
    explicit GameLoop(uint32 framerate, QObject *parent = nullptr);

    uint32 framerate() const;
    void setFramerate(uint32 framerate);

    uint32 effectiveFramerate() const;

    void setCallback(const std::function<void (float)> &callback);

    void run();

public:
    static const uint32 MAX_FPS = 60;

protected:
    void timerEvent(QTimerEvent *) override;

private:
    uint32 m_fps;
    QBasicTimer m_timer;
    QTime m_deltaTime;

    // For framerate estimation
    std::array<int, 10> m_lastDeltas;
    int m_currentDeltaIndex;

    std::function<void (float)> m_callback;
};

#endif // GAMELOOP_H

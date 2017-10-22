#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <functional>

#include <QBasicTimer>
#include <QObject>
#include <QTime>


class GameLoop : public QObject
{
public:
    explicit GameLoop(unsigned int framerate, QObject *parent = nullptr);

    unsigned int framerate() const;
    void setFramerate(unsigned int framerate);

    unsigned int effectiveFramerate() const;

    void setCallback(const std::function<void (float)> &callback);

    void run();

public:
    static const unsigned int MAX_FPS = 60;

protected:
    void timerEvent(QTimerEvent *) override;

private:
    unsigned int m_fps;
    QBasicTimer m_timer;
    QTime m_deltaTime;

    // For framerate estimation
    std::array<int, 10> m_lastDeltas;
    int m_currentDeltaIndex;

    std::function<void (float)> m_callback;
};

#endif // GAMELOOP_H

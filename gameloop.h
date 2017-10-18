#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <functional>

#include <QBasicTimer>
#include <QObject>
#include <QTime>


class GameLoop : public QObject
{
public:
    explicit GameLoop(unsigned int fps, QObject *parent = nullptr);

    unsigned int fps() const;
    void setFps(unsigned int fps);

    void setCallback(const std::function<void (float)> &callback);

    void run();

protected:
    void timerEvent(QTimerEvent *) override;

private:
    unsigned int m_fps;
    QBasicTimer m_timer;
    QTime m_deltaTime;

    std::function<void (float)> m_callback;
};

#endif // GAMELOOP_H

#include "fpswidgets.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QTimer>

#include "core/gameloop.h"


QWidget *createFpsLabel(GameLoop *gameLoop, QWidget *parent)
{
    auto *ret = new QLabel(parent);
    ret->setStyleSheet("QLabel { background-color : red }");
    ret->setFixedWidth(50);

    auto *effectiveFpsTimer = new QTimer(ret);
    effectiveFpsTimer->setInterval(500);

    ret->connect(effectiveFpsTimer, &QTimer::timeout, [ret, gameLoop] {
        const uint32 fps = gameLoop->effectiveFramerate();

        ret->setText(QString::number(fps) + " fps");
    });

    effectiveFpsTimer->start();

    return ret;
}

QWidget *createFpsSlider(GameLoop *gameLoop, QWidget *parent)
{
    auto fpsIndicator = new QLabel(parent);

    auto *ret = new QSlider(parent);
    ret->setOrientation(Qt::Horizontal);
    ret->setMaximum(GameLoop::MAX_FPS);

    auto fpsControlsLayout = new QHBoxLayout;
    fpsControlsLayout->addWidget(fpsIndicator);
    fpsControlsLayout->addWidget(ret);

    ret->connect(ret, &QSlider::valueChanged,
            [gameLoop, fpsIndicator] (int fps) {
        gameLoop->setFramerate(fps);

        const QString fpsText = QString::number(fps);

        fpsIndicator->setText("fps :" + fpsText);
    });

    ret->setValue(gameLoop->framerate());

    return ret;
}

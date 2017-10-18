#include <QApplication>
#include <QSurfaceFormat>

#include "gameloop.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Game Engines - PW3");

    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);

    QSurfaceFormat::setDefaultFormat(format);

    GameLoop gameLoop(60);

    MainWindow mw(&gameLoop);
    mw.setMinimumSize(640, 400);
    mw.show();

    mw.loadHeightMap("images/heightmap-1.png");

    gameLoop.run();

    return app.exec();
}

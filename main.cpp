#include <QApplication>
#include <QSurfaceFormat>

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

    MainWindow mw;
    mw.setMinimumSize(640, 400);
    mw.show();

    mw.loadHeightMap("images/heightmap-1.png");

    return app.exec();
}

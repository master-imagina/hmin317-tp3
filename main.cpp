#include <QApplication>
#include <QSurfaceFormat>

#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Game Engines - PW2");

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow mw;
    mw.setMinimumSize(640, 400);
    mw.show();

    return app.exec();
}

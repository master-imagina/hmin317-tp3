#include <QApplication>
#include <QDebug>
#include <QSurfaceFormat>

#include "mainwindow.h"


int main(int argc, char *argv[])
{
    // Set OpenGL version
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);

    QSurfaceFormat::setDefaultFormat(format);

    // Create app
    QApplication app(argc, argv);
    app.setApplicationName("Game Engines - Editor");

    MainWindow mw;
    mw.setMinimumSize(640, 400);
    mw.show();

    return app.exec();
}

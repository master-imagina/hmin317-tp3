#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>
#include <QTimer>
#include <QSignalMapper>
#include <QObject>
#include <iostream>
#ifndef QT_NO_OPENGL
#include "mainwidget.h"
#endif
int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("plane");
    app.setApplicationVersion("0.1");

    QTimer timer;

    timer.setInterval(4000);

    //QSignalMapper * signalMapper = new QSignalMapper(&timer);

    //QObject::connect(&timer,SIGNAL(timeout()),signalMapper,SLOT( map()));
    //signalMapper->setMapping(&timer,0);
    //QObject::connect(signalMapper,SIGNAL(mapped(int)),&timer,SIGNAL(timeout(int)));

    //connect(this,SIGNAL(timeout(int)),this,SLOT(crossCapture(int)));


    std::cout << "Main instruction" << std::endl;
#ifndef QT_NO_OPENGL
    MainWidget widget1;
    widget1.setWindowTitle("Widget 1");
    widget1.setFPS(120);
    widget1.setTimer(&timer);
    widget1.setSeason(0);
    widget1.setRGB(1.0,1.0,1.0);
    widget1.show();


#else
    QLabel note("OpenGL Support required");
    note.show();
#endif
    timer.start();
    return app.exec();
}

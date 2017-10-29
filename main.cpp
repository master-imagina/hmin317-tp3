#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>
#include <QTimer>

#ifndef QT_NO_OPENGL
#include "mainwidget.h"
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("TP3");
    app.setApplicationVersion("0.1");
#ifndef QT_NO_OPENGL
    MainWidget *widget1 = new MainWidget(nullptr, 30, 1); //1 = printemps
    MainWidget *widget2 = new MainWidget(nullptr, 30, 2); //2 = été
    MainWidget *widget3 = new MainWidget(nullptr, 30, 3); //3 = automne
    MainWidget *widget4 = new MainWidget(nullptr, 30, 4); //4 = hiver

    widget1->show();
    widget2->show();
    widget3->show();
    widget4->show();

    QTimer* seasonTimer = new QTimer;

    QObject::connect(seasonTimer, SIGNAL(timeout()), widget1, SLOT(SaisonSuivante()));
    QObject::connect(seasonTimer, SIGNAL(timeout()), widget2, SLOT(SaisonSuivante()));
    QObject::connect(seasonTimer, SIGNAL(timeout()), widget3, SLOT(SaisonSuivante()));
    QObject::connect(seasonTimer, SIGNAL(timeout()), widget4, SLOT(SaisonSuivante()));

    seasonTimer->start(5000);

#else
    QLabel note("OpenGL Support required");
    note.show();
#endif
    return app.exec();
}

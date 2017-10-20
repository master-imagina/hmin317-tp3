#include <QApplication>

#include "graphicmanager.h"
#include "syncronizer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("RPSoundManager");

    GraphicManager gManager;
    gManager.show();
/*
    GraphicManager gManager1;
    gManager1.show();

    GraphicManager gManager2;
    gManager2.show();

    GraphicManager gManager3;
    gManager3.show();
*/
    Syncronizer syncronizer;

    QObject::connect(&syncronizer, SIGNAL(firstChanged(int)), &gManager, SLOT(changeSaison(int)));/*
    QObject::connect(&syncronizer, SIGNAL(secondChanged(int)), &gManager1, SLOT(changeSaison(int)));
    QObject::connect(&syncronizer, SIGNAL(thirdChanged(int)), &gManager2, SLOT(changeSaison(int)));
    QObject::connect(&syncronizer, SIGNAL(fourthChanged(int)), &gManager3, SLOT(changeSaison(int)));
*/
    QObject::connect(&gManager, SIGNAL(updateSaison()), &syncronizer, SLOT(update()));

    return app.exec();
}

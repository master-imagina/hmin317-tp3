#include "src/glwidget.h"
#include "src/syncronizer.h"
#include "src/gameobject/gameobject.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GLWidget w1(0, 0);
    GLWidget w2(0, 1);
    GLWidget w3(0, 2);
    GLWidget w4(0, 3);
    Syncronizer syncronizer;
    QObject::connect(&syncronizer, SIGNAL(firstChanged(int)), &w1, SLOT(changeSaison(int)));
    QObject::connect(&syncronizer, SIGNAL(secondChanged(int)), &w2, SLOT(changeSaison(int)));
    QObject::connect(&syncronizer, SIGNAL(thirdChanged(int)), &w3, SLOT(changeSaison(int)));
    QObject::connect(&syncronizer, SIGNAL(fourthChanged(int)), &w4, SLOT(changeSaison(int)));
    QObject::connect(&w4, SIGNAL(updateSaison()), &syncronizer, SLOT(update()));
    w1.show();
    w2.show();
    w3.show();
    w4.show();

    return a.exec();
}

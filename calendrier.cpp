#include "calendrier.h"
#include <QTimer>
#include <iostream>

Calendrier::Calendrier(QObject *parent) : QObject(parent)
{

}

void Calendrier::calendrierChangeSaison() {
    std::cout << "Changement de saison" << std::endl;
    emit signalSaison();
}

void Calendrier::timer() {
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(calendrierChangeSaison()));
    timer->start(5000);
}

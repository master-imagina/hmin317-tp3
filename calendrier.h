#ifndef CALENDRIER_H
#define CALENDRIER_H

#include <QObject>

class Calendrier : public QObject
{
    Q_OBJECT
public:
    explicit Calendrier(QObject *parent = nullptr);
    void timer();

signals:
    void signalSaison();

public slots:
    void calendrierChangeSaison();
};

#endif // CALENDRIER_H

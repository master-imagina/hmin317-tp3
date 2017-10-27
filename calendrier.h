#ifndef CALENDAR_H
#define CALENDAR_H

#include <QObject>
#include <QCalendarWidget>
#include <QBasicTimer>
#include <iostream>

class Calendrier : public QCalendarWidget
{
    Q_OBJECT
public:
    explicit Calendrier(QWidget* parent = 0, int dps = 0);
    ~Calendrier();

protected:
    void timerEvent(QTimerEvent *e);
    void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const;

signals:
    void swapS();

private:
    QBasicTimer timer;
    QDate date;

    int dps;
};

#endif // CALENDAR_H

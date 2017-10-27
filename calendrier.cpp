#include "calendrier.h"
#include <fstream>
#include <iostream>

using namespace std;


Calendrier::Calendrier(QWidget* parent, int dps) :
    QCalendarWidget(parent)
{
    date = QCalendarWidget::selectedDate();
    cout << date.toString("dd-MM-yy").toStdString() << endl;
    timer.start(1000 / dps, this);
}

Calendrier::~Calendrier()
{

}

void Calendrier::timerEvent(QTimerEvent *)
{
    date = date.addDays(1);
    cout << date.toString("dd-MM-yy").toStdString() << endl;
    if(date.day() == 1 && date.month() % 3 == 1) {
        cout << "Nouvelle saison." << endl;
        emit swapS();
    }
    update();
}

void Calendrier::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
    /*if ( ) { // our conditions
        // When the conditions are matched, passed QDate is drawn as we like.
        painter->save();
        painter->drawEllipse(rect); // here we draw n ellipse and the day—
        painter->drawText(rec, Qt::TextSingleLine, Qt::AlignCenter, QString::number(date.day()));
        painter->restore();
    } else { // if our conditions are not matching, show the default way.
        QCalendarWidget::paintCell(painter, rect, date);
    }*/
}

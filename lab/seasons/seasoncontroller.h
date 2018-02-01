#ifndef SEASONCONTROLLER_H
#define SEASONCONTROLLER_H

#include <QTimer>
#include <QDate>


enum class Season
{
    None,
    Autumn,
    Winter,
    Spring,
    Summer
};


class SeasonController : public QObject
{
    Q_OBJECT

public:
    SeasonController(QObject *parent = nullptr);

    void start();

Q_SIGNALS:
    void seasonChanged(Season season);

private:
    void initSeasons();

private:
    Season m_currentSeason;
    QDate m_currentDate;
    QTimer *m_seasonTimer;
};

#endif // SEASONCONTROLLER_H

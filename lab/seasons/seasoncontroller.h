#ifndef SEASONCONTROLLER_H
#define SEASONCONTROLLER_H

#include <array>

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

    QColor colorFromSeason() const;

Q_SIGNALS:
    void seasonChanged(const QColor &color);

private:
    void initSeasons();

    QColor colorFromSeason(Season season) const;

private:
    Season m_currentSeason;
    QDate m_currentDate;
    QTimer *m_seasonTimer;
};

#endif // SEASONCONTROLLER_H

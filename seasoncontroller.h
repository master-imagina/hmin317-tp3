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
public:
    SeasonController(QObject *parent = nullptr);

    void start();

    QColor colorFromSeason(int viewportId);

private:
    void initSeasons();

    QColor colorFromSeason(Season season);

private:
    std::array<Season, 4> m_currentSeasons;
    std::array<QDate, 4> m_currentDates;
    QTimer *m_seasonTimer;
};

#endif // SEASONCONTROLLER_H

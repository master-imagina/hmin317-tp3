#ifndef SEASONMANAGER_H
#define SEASONMANAGER_H

#include <QObject>
#include <QColor>

typedef enum class Seasons {
    Summer,
    Autumn,
    Winter,
    Spring,
    EOL
} Seasons;

class SeasonManager : public QObject
{
    Q_OBJECT
public:
    explicit SeasonManager(Seasons s);
    ~SeasonManager();
    Seasons getSeason();
    QColor getCurrentSeasonColor();
    QColor getNextSeasonColor();
public slots:
    void changeSeason();
private:
    Seasons currentSeason;

    QColor getSeasonColor(Seasons season);
};

#endif // SEASONMANAGER_H

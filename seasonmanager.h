#ifndef SEASONMANAGER_H
#define SEASONMANAGER_H

#include <QObject>
#include <QColor>

typedef enum class Season{
    Summer,
    Autumn,
    Winter,
    Spring,
    EOL
}Season;

class SeasonManager : public QObject
{
    Q_OBJECT
public:
    explicit SeasonManager(Season s);
    ~SeasonManager();

    Season getSeason();
    QColor getCurrentSeasonColor();
    QColor getNextSeasonColor();

public slots:
    void updtateSeason();

private:
    Season currentSeason;

    QColor getSeasonColor(Season s);
};

#endif // SEASONMANAGER_H

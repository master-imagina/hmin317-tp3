#ifndef SEASONMANAGER_H
#define SEASONMANAGER_H

#include <QObject>

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
public slots:
    void changeSeason();
private:
    Seasons currentSeason;
};

#endif // SEASONMANAGER_H

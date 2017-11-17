#include "seasonmanager.h"

SeasonManager::SeasonManager(Season s)
    :currentSeason(s)
{

}

SeasonManager::~SeasonManager()
{

}

Season& operator ++(Season &s)
{
    s = static_cast<Season>(static_cast<int>(s) + 1);
    if(s == Season::EOL){
        s = static_cast<Season>(0);
    }
    return s;
}

Season operator ++(Season &s, int)
{
    Season r = s;
    r++;
    return r;
}

void SeasonManager::updtateSeason()
{
    ++currentSeason;
}

Season SeasonManager::getSeason()
{
    return currentSeason;
}

QColor SeasonManager::getCurrentSeasonColor()
{
    return getSeasonColor(currentSeason);
}

QColor SeasonManager::getNextSeasonColor()
{
    Season s = ++currentSeason;
    return getSeasonColor(s);
}

QColor SeasonManager::getSeasonColor(Season s)
{
    switch (s) {
        case Season::Autumn:
            return QColor(qRgb(204,102,0));
            break;
        case Season::Winter:
            return QColor(qRgb(77,210,255));
            break;
        case Season::Spring:
            return QColor(qRgb(0,204,0));
            break;
        case Season::Summer:
            return QColor(qRgb(255,255,77));
            break;
    }
}

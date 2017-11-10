#include "seasonmanager.h"

SeasonManager::SeasonManager(Season s)
    :currentSeason(s)
{

}

Season& operator ++(Season &s){
    s = static_cast<Season>(static_cast<int>(s) + 1);
    if(s == Season::EOL){
        s = static_cast<Season>(0);
    }
    return s;
}

Season& operator ++(Season &s, int){
    Seasons r = s;
        r++;
        return r;
}

void SeasonManager::updtateSeason()
{

}

Season SeasonManager::getSeason()
{

}

QColor SeasonManager::getCurrentSeasonColor()
{

}

QColor SeasonManager::getNextSeasonColor()
{

}

QColor SeasonManager::getSeasonColor(Season s)
{

}

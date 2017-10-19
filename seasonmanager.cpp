#include "seasonmanager.h"
#include <iostream>

SeasonManager::SeasonManager(Seasons s)
        : currentSeason(s)
{

}

SeasonManager::~SeasonManager() {

}

Seasons& operator++(Seasons &s) {
    //using IntType = typename std::underlying_type<Seasons>::type;
    s = static_cast<Seasons>(static_cast<int>(s) + 1);
    if(s == Seasons::EOL) {
        s = static_cast<Seasons>(0);
    }
    return s;
}

Seasons operator++(Seasons &s, int) {
    Seasons r = s;
    ++r;
    return r;
}

void SeasonManager::changeSeason() {
    ++currentSeason;
}

Seasons SeasonManager::getSeason() {
    return currentSeason;
}

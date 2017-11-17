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

QColor SeasonManager::getSeasonColor(Seasons season) {
    switch(season) {
        case Seasons::Autumn :
            return QColor(qRgb(114,73,61));
            break;
        case Seasons::Winter :
            return QColor(qRgb(200,228,250));
            break;
        case Seasons::Summer :
            return QColor(qRgb(255,215,0));
            break;
        case Seasons::Spring :
            return QColor(qRgb(110,214,107));
            break;
    }
}

QColor SeasonManager::getCurrentSeasonColor() {
    return getSeasonColor(currentSeason);
}

QColor SeasonManager::getNextSeasonColor() {
    Seasons s = static_cast<Seasons>(static_cast<int>(currentSeason) + 1);
    if(s == Seasons::EOL) {
        s = static_cast<Seasons>(0);
    }
    return getSeasonColor(s);
}

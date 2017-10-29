#include "seasoncontroller.h"

#include <QColor>


SeasonController::SeasonController(QObject *parent) :
    QObject(parent),
    m_currentSeasons(),
    m_currentDates(),
    m_seasonTimer(new QTimer(this))
{
    initSeasons();
}

void SeasonController::start()
{
    m_seasonTimer->start();
}

QColor SeasonController::colorFromSeason(int viewportId)
{
    Q_ASSERT (viewportId < 4);

    return colorFromSeason(m_currentSeasons[viewportId]);
}

void SeasonController::initSeasons()
{
    static const std::map<std::pair<int, int>, Season> DATE_TO_SEASON {
        {{9, 22}, Season::Autumn},
        {{12, 21}, Season::Winter},
        {{3, 20}, Season::Spring},
        {{6, 21}, Season::Summer}
    };

    // Init seasons
    for (auto it = DATE_TO_SEASON.begin(); it != DATE_TO_SEASON.end(); it++) {
        const int i = DATE_TO_SEASON.size() - std::distance(it, DATE_TO_SEASON.end());

        m_currentDates[i] = QDate(2017, it->first.first, it->first.second - 1);
        m_currentSeasons[i] = it->second;
    }

    // Init and start timer
    m_seasonTimer->setInterval(75);

    connect(m_seasonTimer, &QTimer::timeout, [this] {
        for (int i = 0; i < m_currentDates.size(); i++) {
            QDate &date = m_currentDates[i];
            date = date.addDays(1);

            const std::pair<int, int> dayAndMonth {date.month(), date.day()};

            auto seasonChangedIt = DATE_TO_SEASON.find(dayAndMonth);

            if (seasonChangedIt != DATE_TO_SEASON.end()) {
                const Season newSeason = seasonChangedIt->second;

                m_currentSeasons[i] = newSeason;
            }
        }
    });
}

QColor SeasonController::colorFromSeason(Season season)
{
    QColor ret;

    switch (season) {
    case Season::Autumn:
        ret = QColor(244, 183, 51);
        break;
    case Season::Winter:
        ret = Qt::white;
        break;
    case Season::Spring:
        ret = Qt::green;
        break;
    case Season::Summer:
        ret = Qt::yellow;
        break;
    default:
        break;
    }

    return ret;
}


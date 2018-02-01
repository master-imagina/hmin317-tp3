#include "hueslider.h"

#include <QPainter>


////////////////////////// HueSlider //////////////////////////

HueSlider::HueSlider(QWidget *parent) :
    PrettySlider(parent),
    m_gradient()
{
    setRange(0, 359);
}

HueSlider::HueSlider(Qt::Orientation orientation, QWidget *parent) :
    PrettySlider(orientation, parent),
    m_gradient()
{
    setRange(0, 359);
}

HueSlider::~HueSlider()
{}

void HueSlider::resizeEvent(QResizeEvent *)
{
    // Update the hue gradient
    QPoint startPt(0.0, height());
    QPoint endPt(0.0, 0.0);

    if (orientation() == Qt::Horizontal) {
        startPt = QPoint(0.0, 0.0);
        endPt = QPoint(width(), 0.0);
    }

    m_gradient = QLinearGradient(startPt, endPt);

    qreal nextGradientStop = 0;

    for (int nextHue = 0; nextHue <= 360; nextHue += 60) {
        if (nextHue > 359) {
            nextHue = 359;
        }

        m_gradient.setColorAt(nextGradientStop, QColor::fromHsv(nextHue, 255, 255));

        nextGradientStop += 1.0 / 6;
    }
}

void HueSlider::drawBackground(QPainter *painter, const QRect &rect, int radius) const
{
    painter->setPen(QPen(Qt::black, 0.5));

    painter->setBrush(m_gradient);
    painter->drawRoundedRect(rect, radius, radius);
}

void HueSlider::drawHandleBackground(QPainter *painter, const QRect &rect, int radius) const
{
    painter->setBrush(QColor::fromHsv(value(), 255, 255));
    painter->drawRoundedRect(rect, radius,radius);
}

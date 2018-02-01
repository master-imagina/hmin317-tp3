#include "saturationslider.h"

#include <QPainter>


////////////////////////// SaturationSlider //////////////////////////

SaturationSlider::SaturationSlider(QWidget *parent) :
    PrettySlider(parent),
    m_hue(0),
    m_gradient()
{
    setRange(0, 255);
}

SaturationSlider::SaturationSlider(Qt::Orientation orientation, QWidget *parent) :
    PrettySlider(orientation, parent),
    m_hue(0),
    m_gradient()
{
    setRange(0, 255);
}

void SaturationSlider::hsv(int *h, int *s, int *v) const
{
    *h = m_hue;
    *s = value();
    *v = 255;
}

void SaturationSlider::setHue(int h)
{
    if (m_hue != h) {
        m_hue = h;

        updateGradient();
        update();
    }
}

void SaturationSlider::resizeEvent(QResizeEvent *)
{
   updateGradient();
   update();
}

void SaturationSlider::drawBackground(QPainter *painter, const QRect &rect, int radius) const
{
    painter->setPen(QPen(Qt::black, 0.5));

    painter->setBrush(m_gradient);
    painter->drawRoundedRect(rect, radius, radius);
}

void SaturationSlider::drawHandleBackground(QPainter *painter, const QRect &rect, int radius) const
{
    painter->setBrush(QColor::fromHsv(m_hue, value(), 255));
    painter->drawRoundedRect(rect, radius,radius);
}

void SaturationSlider::updateGradient()
{
    QPoint startPt(0.0, height());
    QPoint endPt(0.0, 0.0);

    if (orientation() == Qt::Horizontal) {
        startPt = QPoint(0.0, 0.0);
        endPt = QPoint(width(), 0.0);
    }

    m_gradient = QLinearGradient(startPt, endPt);

    m_gradient.setColorAt(0.0, QColor::fromHsv(m_hue, 0, 255));
    m_gradient.setColorAt(1.0, QColor::fromHsv(m_hue, 255, 255));
}

#include "valueslider.h"

#include <QPainter>


////////////////////////// ValueSlider //////////////////////////

ValueSlider::ValueSlider(QWidget *parent) :
    PrettySlider(parent),
    m_hue(0),
    m_gradient()
{
    setRange(0, 255);
}

ValueSlider::ValueSlider(Qt::Orientation orientation, QWidget *parent) :
    PrettySlider(orientation, parent),
    m_hue(0),
    m_gradient()
{
    setRange(0, 255);
}

void ValueSlider::hsv(int *h, int *s, int *v) const
{
    *h = m_hue;
    *s = 255;
    *v = value();
}

void ValueSlider::setHue(int h)
{
    if (m_hue != h) {
        m_hue = h;

        updateGradient();
        update();
    }
}

void ValueSlider::resizeEvent(QResizeEvent *)
{
    updateGradient();
    update();
}

void ValueSlider::drawBackground(QPainter *painter, const QRect &rect, int radius) const
{
    painter->setPen(QPen(Qt::black, 0.5));

    painter->setBrush(m_gradient);
    painter->drawRoundedRect(rect, radius, radius);
}

void ValueSlider::drawHandleBackground(QPainter *painter, const QRect &rect, int radius) const
{
    painter->setBrush(QColor::fromHsv(m_hue, 255, value()));
    painter->drawRoundedRect(rect, radius,radius);
}

void ValueSlider::updateGradient()
{
    QPoint startPt(0.0, height());
    QPoint endPt(0.0, 0.0);

    if (orientation() == Qt::Horizontal) {
        startPt = QPoint(0.0, 0.0);
        endPt = QPoint(width(), 0.0);
    }

    m_gradient = QLinearGradient(startPt, endPt);

    m_gradient.setColorAt(0.0, QColor::fromHsv(m_hue, 255, 0));
    m_gradient.setColorAt(1.0, QColor::fromHsv(m_hue, 255, 255));
}

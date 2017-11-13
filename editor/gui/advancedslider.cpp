#include "advancedslider.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionSlider>


////////////////////// AdvancedSlider //////////////////////

AdvancedSlider::AdvancedSlider(QWidget *parent) :
    QSlider(parent)
{}

AdvancedSlider::AdvancedSlider(Qt::Orientation orientation, QWidget *parent) :
    QSlider(orientation, parent)
{}

void AdvancedSlider::setValueAtomic(int newValue)
{
    const QSignalBlocker blocker(this);

    setValue(newValue);
}

void AdvancedSlider::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        const int range = maximum() - minimum();

        //FIXME when selecting the handle if its to the max
        if (orientation() == Qt::Vertical) {
            setValue(minimum() + range * (height() - e->y()) / height()) ;
        }
        else {
            setValue(minimum() + range * e->x() / width());
        }

        e->accept();
    }

    QSlider::mousePressEvent(e);
}


////////////////////// PrettySlider //////////////////////

PrettySlider::PrettySlider(QWidget *parent) :
    AdvancedSlider(parent)
{}

PrettySlider::PrettySlider(Qt::Orientation orientation, QWidget *parent) :
    AdvancedSlider(orientation, parent)
{}

void PrettySlider::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    painter.save();

    // Draw background
    const QRect myRect = rect().adjusted(3, 0, -3, 0);
    const int rectRadius = 3;

    drawBackground(&painter, myRect, rectRadius);

    painter.restore();

    // Draw handle
    QStyleOptionSlider opt;
    initStyleOption(&opt);

    QRect handleRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle);
    handleRect.adjust(1, 1, -1, -1);
    const int handleRadius = 7;

    QPen pen(Qt::white);
    pen.setWidth(2);

    painter.setPen(pen);
    drawHandleBackground(&painter, handleRect, handleRadius);
}

void PrettySlider::drawBackground(QPainter *painter, const QRect &rect, int radius) const
{
    Q_UNUSED(painter);
    Q_UNUSED(rect);
    Q_UNUSED(radius);
}

void PrettySlider::drawHandleBackground(QPainter *painter, const QRect &rect, int radius) const
{
    Q_UNUSED(painter);
    Q_UNUSED(rect);
    Q_UNUSED(radius);
}


////////////////////// ValuedSlider //////////////////////

ValuedSlider::ValuedSlider(QWidget *parent) :
    AdvancedSlider(parent)
{}

ValuedSlider::ValuedSlider(Qt::Orientation orientation, QWidget *parent) :
    AdvancedSlider(orientation, parent)
{}

void ValuedSlider::paintEvent(QPaintEvent *e)
{
    QStyleOptionSlider opt;
    initStyleOption(&opt);

    QRect handleRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle);
    handleRect.adjust(0, 0, 0, 3);

    QPainter p(this);
    p.fillRect(0, 0, handleRect.center().x(), handleRect.bottom(), palette().dark());
    p.drawText(handleRect, QString::number(value()));
}

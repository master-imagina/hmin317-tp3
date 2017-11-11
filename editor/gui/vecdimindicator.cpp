#include "vecdimindicator.h"

#include <QPaintEvent>
#include <QPainter>


VectorDimIndicator::VectorDimIndicator(QWidget *parent) :
    QWidget(parent),
    m_text(),
    m_color()
{}

VectorDimIndicator::VectorDimIndicator(const QString &text,
                                       const QColor &color,
                                       QWidget *parent) :
    QWidget(parent),
    m_text(text),
    m_color(color)
{
    m_text = text;
    m_color = color;
}

VectorDimIndicator::~VectorDimIndicator()
{}

QString VectorDimIndicator::text() const
{
    return m_text;
}

void VectorDimIndicator::setText(const QString &text)
{
    if (m_text != text) {
        m_text = text;

        Q_EMIT textChanged(text);
    }
}

QColor VectorDimIndicator::color() const
{
    return m_color;
}

void VectorDimIndicator::setColor(const QColor &color)
{
    if (m_color != color) {
        m_color = color;

        Q_EMIT colorChanged(color);
    }
}

QSize VectorDimIndicator::sizeHint() const
{
    const QFontMetrics fm(fontMetrics());

    const int w = fm.width(m_text) + 6;
    const int h = fm.height();

    QSize ret(w, h);

    return ret;
}

QSize VectorDimIndicator::minimumSizeHint() const
{
    return sizeHint();
}

void VectorDimIndicator::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    const QRect eventRect = event->rect();

    const int rectRadius = 2;

    p.save();

    p.setPen(m_color);
    p.setBrush(m_color);
    p.drawRoundedRect(eventRect.adjusted(0, 0, -1, -1),
                      rectRadius, rectRadius);

    p.restore();

    p.setPen(Qt::white);
    p.drawText(eventRect, Qt::AlignHCenter | Qt::AlignVCenter, m_text);
}

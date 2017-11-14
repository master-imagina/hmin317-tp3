#ifndef VALUESLIDER_H
#define VALUESLIDER_H

#include "editor/gui/advancedslider.h"


class ValueSlider : public PrettySlider
{
    Q_OBJECT

public:
    explicit ValueSlider(QWidget *parent = nullptr);
    ValueSlider(Qt::Orientation orientation, QWidget *parent = nullptr);

    void hsv(int *h, int *s, int *v) const;
    void setHue(int h);

protected:
    void resizeEvent(QResizeEvent *) override;

    void drawBackground(QPainter *painter, const QRect &rect, int radius) const override;
    void drawHandleBackground(QPainter *painter, const QRect &rect, int radius) const override;

private:
    void updateGradient();

private:
    int m_hue;
    QLinearGradient m_gradient;
};

#endif // VALUESLIDER_H

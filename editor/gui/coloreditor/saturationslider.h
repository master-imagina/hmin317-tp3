#ifndef SATURATIONSLIDER_H
#define SATURATIONSLIDER_H

#include "../advancedslider.h"


class SaturationSlider : public PrettySlider
{
    Q_OBJECT

public:
    explicit SaturationSlider(QWidget *parent = nullptr);
    SaturationSlider(Qt::Orientation orientation, QWidget *parent = nullptr);

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

#endif // SATURATIONSLIDER_H

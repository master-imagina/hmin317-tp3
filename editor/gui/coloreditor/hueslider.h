#ifndef HUESLIDER_H
#define HUESLIDER_H

#include "editor/gui/advancedslider.h"


class HueSlider : public PrettySlider
{
    Q_OBJECT

public:
    explicit HueSlider(QWidget *parent = nullptr);
    HueSlider(Qt::Orientation orientation, QWidget *parent = nullptr);
    ~HueSlider();

protected:
    void resizeEvent(QResizeEvent *) override;

    void drawBackground(QPainter *painter, const QRect &rect, int radius) const override;
    void drawHandleBackground(QPainter *painter, const QRect &rect, int radius) const override;

private:
    QLinearGradient m_gradient;
};

#endif // HUESLIDER_H

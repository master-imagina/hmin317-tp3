#ifndef ADVANDEDSLIDER_H
#define ADVANDEDSLIDER_H

#include <QSlider>


class AdvancedSlider : public QSlider
{
    Q_OBJECT

public:
    explicit AdvancedSlider(QWidget *parent = nullptr);
    AdvancedSlider(Qt::Orientation orientation, QWidget *parent = nullptr);

    void setValueAtomic(int newValue);

protected:
    void mousePressEvent(QMouseEvent *e) override;
};


class PrettySlider : public AdvancedSlider
{
public:
    explicit PrettySlider(QWidget *parent = nullptr);
    PrettySlider(Qt::Orientation orientation, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *e) override;

    virtual void drawBackground(QPainter *painter, const QRect &rect, int radius) const;
    virtual void drawHandleBackground(QPainter *painter, const QRect &rect, int radius) const;
};

#endif // ADVANDEDSLIDER_H

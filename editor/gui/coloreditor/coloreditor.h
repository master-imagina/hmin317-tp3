#ifndef COLOREDITOR_H
#define COLOREDITOR_H

#include <QWidget>

class HueSlider;
class SaturationSlider;
class ValueSlider;


class ColorEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ColorEditor(QWidget *parent = nullptr);

    QColor value() const;
    void setValue(const QColor &color);

Q_SIGNALS:
    void valueChanged(const QColor &color);

private:
    void onHueChanged(int hue);
    void onSaturationChanged(int saturation);
    void onValueChanged(int value);

private:
    HueSlider *m_hueSlider;
    SaturationSlider *m_saturationSlider;
    ValueSlider *m_valueSlider;
};

#endif // COLOREDITOR_H

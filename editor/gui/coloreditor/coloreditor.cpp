#include "coloreditor.h"

#include <QDebug>
#include <QVBoxLayout>

#include "hueslider.h"
#include "saturationslider.h"
#include "valueslider.h"


ColorEditor::ColorEditor(QWidget *parent) :
    QWidget(parent),
    m_hueSlider(nullptr),
    m_saturationSlider(nullptr),
    m_valueSlider(nullptr)
{
    m_hueSlider = new HueSlider(Qt::Horizontal, this);
    m_saturationSlider = new SaturationSlider(Qt::Horizontal, this);
    m_valueSlider = new ValueSlider(Qt::Horizontal, this);

    connect(m_hueSlider, &QSlider::valueChanged,
            this, &ColorEditor::onHueChanged);
    connect(m_saturationSlider, &QSlider::valueChanged,
            this, &ColorEditor::onSaturationChanged);
    connect(m_valueSlider, &QSlider::valueChanged,
            this, &ColorEditor::onValueChanged);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->addWidget(m_hueSlider);
    mainLayout->addWidget(m_saturationSlider);
    mainLayout->addWidget(m_valueSlider);
    mainLayout->addStretch();
}

QColor ColorEditor::value() const
{
    return {
        m_hueSlider->value(),
        m_saturationSlider->value(),
        m_valueSlider->value()
    };
}

void ColorEditor::setValue(const QColor &color)
{
    const QColor currentColor = value();

    if (currentColor != color) {
        m_hueSlider->setValueAtomic(color.hue());
        m_saturationSlider->setValueAtomic(color.saturation());
        m_valueSlider->setValueAtomic(color.value());
    }

    Q_EMIT valueChanged(color);
}

void ColorEditor::onHueChanged(int hue)
{
    m_saturationSlider->setHue(hue);
    m_valueSlider->setHue(hue);

    setValue(QColor::fromHsv(hue,
                             m_saturationSlider->value(),
                             m_valueSlider->value()));
}

void ColorEditor::onSaturationChanged(int saturation)
{
    setValue(QColor::fromHsv(m_hueSlider->value(),
                             saturation,
                             m_valueSlider->value()));
}

void ColorEditor::onValueChanged(int value)
{
    setValue(QColor::fromHsv(m_hueSlider->value(),
                             m_saturationSlider->value(),
                             value));
}

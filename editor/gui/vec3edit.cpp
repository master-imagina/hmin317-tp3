#include "vec3edit.h"

#include <QDoubleSpinBox>
#include <QHBoxLayout>

#include "vecdimindicator.h"


Vec3DEdit::Vec3DEdit(QWidget *parent) :
    QWidget(parent),
    m_xSpinBox(new QDoubleSpinBox(this)),
    m_ySpinBox(new QDoubleSpinBox(this)),
    m_zSpinBox(new QDoubleSpinBox(this))
{
    setFocusPolicy(Qt::StrongFocus);
    setFocusProxy(m_xSpinBox);
    setTabOrder(m_xSpinBox, m_ySpinBox);
    setTabOrder(m_ySpinBox, m_zSpinBox);

    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto xIndicator = new VectorDimIndicator("X", Qt::red, this);
    auto yIndicator = new VectorDimIndicator("Y",
                                             QColor::fromRgbF(0.24, 0.73, 0.13),
                                             this);
    auto zIndicator = new VectorDimIndicator("Z",
                                             QColor::fromRgbF(0.26, 0.54, 1.0),
                                             this);

    static const int SPACE_BETWEEN_SPINBOXES = 7;

    layout->addWidget(xIndicator, 0, Qt::AlignLeft);
    layout->addWidget(m_xSpinBox, 1);
    layout->addSpacing(SPACE_BETWEEN_SPINBOXES);

    layout->addWidget(yIndicator, 0, Qt::AlignLeft);
    layout->addWidget(m_ySpinBox, 1);
    layout->addSpacing(SPACE_BETWEEN_SPINBOXES);

    layout->addWidget(zIndicator, 0, Qt::AlignLeft);
    layout->addWidget(m_zSpinBox, 1);

    auto valueChangedSig = static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged);

    connect(m_xSpinBox, valueChangedSig,
            this, [=] (double val) {
        Q_EMIT valueChanged(QVector3D(static_cast<float>(val),
                                      m_ySpinBox->value(),
                                      m_zSpinBox->value()));
    });

    connect(m_ySpinBox, valueChangedSig,
            this, [=] (double val) {
        Q_EMIT valueChanged(QVector3D(m_xSpinBox->value(),
                                      static_cast<float>(val),
                                      m_zSpinBox->value()));
    });

    connect(m_zSpinBox, valueChangedSig,
            this, [=] (double val) {
        Q_EMIT valueChanged(QVector3D(m_xSpinBox->value(),
                                      m_ySpinBox->value(),
                                      static_cast<float>(val)));
    });
}

Vec3DEdit::~Vec3DEdit()
{}

QVector3D Vec3DEdit::value() const
{
    return QVector3D(m_xSpinBox->value(),
                     m_ySpinBox->value(),
                     m_zSpinBox->value());
}

void Vec3DEdit::setValue(const QVector3D &val)
{
    if (value() != val) {
        m_xSpinBox->setValue(val.x());
        m_ySpinBox->setValue(val.y());
        m_zSpinBox->setValue(val.z());

        Q_EMIT valueChanged(val);
    }
}

int Vec3DEdit::decimals() const
{
    return m_xSpinBox->decimals();
}

void Vec3DEdit::setDecimals(int prec)
{
    if (decimals() != prec) {
        m_xSpinBox->setDecimals(prec);
        m_ySpinBox->setDecimals(prec);
        m_zSpinBox->setDecimals(prec);

        Q_EMIT decimalsChanged(prec);
    }
}

float Vec3DEdit::xMin() const
{
    return m_xSpinBox->minimum();
}

void Vec3DEdit::setXMin(float min)
{
    if (xMin() != min) {
        m_xSpinBox->setMinimum(min);

        Q_EMIT xMinChanged(min);
    }
}

float Vec3DEdit::xMax() const
{
    return m_xSpinBox->maximum();
}

void Vec3DEdit::setXMax(float max)
{
    if (xMax() != max) {
        m_xSpinBox->setMaximum(max);

        Q_EMIT xMaxChanged(max);
    }
}

float Vec3DEdit::yMin() const
{
    return m_ySpinBox->minimum();
}

void Vec3DEdit::setYMin(float min)
{
    if (yMin() != min) {
        m_ySpinBox->setMinimum(min);

        Q_EMIT yMinChanged(min);
    }
}

float Vec3DEdit::yMax() const
{
    return m_ySpinBox->maximum();
}

void Vec3DEdit::setYMax(float max)
{
    if (yMax() != max) {
        m_ySpinBox->setMaximum(max);

        Q_EMIT yMaxChanged(max);
    }
}

float Vec3DEdit::zMin() const
{
    return m_zSpinBox->minimum();
}

void Vec3DEdit::setZMin(float min)
{
    if (zMin() != min) {
        m_zSpinBox->setMinimum(min);

        Q_EMIT zMinChanged(min);
    }
}

float Vec3DEdit::zMax() const
{
    return m_zSpinBox->maximum();
}

void Vec3DEdit::setZMax(float max)
{
    if (zMax() != max) {
        m_zSpinBox->setMaximum(max);

        Q_EMIT zMaxChanged(max);
    }
}

float Vec3DEdit::stepSize() const
{
    return m_xSpinBox->singleStep();
}

void Vec3DEdit::setStepSize(float step)
{
    if (m_xSpinBox->singleStep() != step) {
        m_xSpinBox->setSingleStep(step);
        m_ySpinBox->setSingleStep(step);
        m_zSpinBox->setSingleStep(step);

        Q_EMIT stepSizeChanged(step);
    }
}

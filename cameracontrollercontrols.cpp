#include "cameracontrollercontrols.h"

#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>

#include "cameracontroller.h"


CameraControllerControls::CameraControllerControls(CameraController *controller,
                                                   QWidget *parent) :
    QWidget(parent),
    m_controller(controller)
{
    auto myLayout = new QHBoxLayout(this);

    auto moveSpeedSpinBox = new QDoubleSpinBox(this);
    moveSpeedSpinBox->setMaximum(1000.f);
    moveSpeedSpinBox->setValue(m_controller->moveSpeed());

    connect(moveSpeedSpinBox, static_cast<void (QDoubleSpinBox::*) (double)>(&QDoubleSpinBox::valueChanged),
            [this] (double value) {
        m_controller->setMoveSpeed(value);
    });

    auto linearSpeedSpinBox = new QDoubleSpinBox(this);
    linearSpeedSpinBox->setMaximum(1000.f);
    linearSpeedSpinBox->setValue(m_controller->linearSpeed());

    connect(linearSpeedSpinBox, static_cast<void (QDoubleSpinBox::*) (double)>(&QDoubleSpinBox::valueChanged),
            [this] (double value) {
        m_controller->setLinearSpeed(value);
    });

    auto turboFactorSpinBox = new QDoubleSpinBox(this);
    turboFactorSpinBox->setMaximum(1000.f);
    turboFactorSpinBox->setValue(m_controller->turboFactor());

    connect(turboFactorSpinBox, static_cast<void (QDoubleSpinBox::*) (double)>(&QDoubleSpinBox::valueChanged),
            [this] (double value) {
        m_controller->setTurboFactor(value);
    });

    myLayout->addWidget(new QLabel("Move speed : "));
    myLayout->addWidget(moveSpeedSpinBox);
    myLayout->addWidget(new QLabel("Linear speed : "));
    myLayout->addWidget(linearSpeedSpinBox);
    myLayout->addWidget(new QLabel("Turbo factor : "));
    myLayout->addWidget(turboFactorSpinBox);
    myLayout->addStretch();
}

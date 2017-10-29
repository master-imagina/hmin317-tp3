#include "cameracontrols.h"

#include <QHBoxLayout>

#include "cameraactions.h"
#include "dragtoolbutton.h"


CameraControls::CameraControls(Camera *camera, QWidget *parent) :
    QFrame(parent),
    m_camera(camera),
    m_lookupSpeed(0.1f),
    m_linearSpeed(1.f)
{
    const QString btnStyleSheet = "QToolButton"
                                  "{"
                                  "background-color: rgb(225,225,225); "
                                  "border: 1px solid black; "
                                  "border-radius: 2px"
                                  "}";
    const QSize btnSize {36, 36};

    // Draw view button
    auto dragBtn = new DragToolButton(this);
    dragBtn->setStyleSheet(btnStyleSheet);
    dragBtn->setFixedSize(btnSize);
    dragBtn->setIcon(QIcon(":/editor/res/icons/pan_icon_32.png"));
    dragBtn->setToolTip("Drag view");

    connect(dragBtn, &DragToolButton::mouseDragged,
            [this] (float dx, float dy) {
        truckCamera(m_camera, -dx * m_linearSpeed, dy * m_linearSpeed);
    });

    // Move forward/backward button
    auto moveForwardBtn = new DragToolButton(this);
    moveForwardBtn->setStyleSheet(btnStyleSheet);
    moveForwardBtn->setFixedSize(btnSize);
    moveForwardBtn->setIcon(QIcon(":/editor/res/icons/move_forward_icon_32.png"));
    moveForwardBtn->setToolTip("Move Forward/Backward");

    connect(moveForwardBtn, &DragToolButton::mouseDragged,
            [this] (float dx, float dy) {
        Q_UNUSED (dx);

        moveCameraForward(m_camera, dy * m_linearSpeed);
    });

    // Assemble
    auto *myLayout = new QHBoxLayout(this);
    myLayout->setContentsMargins(0, 0, 0, 0);

    myLayout->addStretch();

    myLayout->addWidget(dragBtn);
    myLayout->addWidget(moveForwardBtn);
}

Camera *CameraControls::getCamera() const
{
    return m_camera;
}

void CameraControls::setCamera(Camera *camera)
{
    if (m_camera != camera) {
        m_camera = camera;
    }
}

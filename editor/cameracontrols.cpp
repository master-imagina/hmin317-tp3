#include "cameracontrols.h"

#include <QComboBox>
#include <QHBoxLayout>

#include "core/scene.h"

#include "editor/gui/dragtoolbutton.h"
#include "editor/gui/gamewidget.h"

#include "extras/cameraactions.h"

#include "render/camera.h"


namespace {

const char FREE_CAMERA_NAME[] = "Free camera";

} // anon namespace


////////////////////// CameraControls //////////////////////

CameraControls::CameraControls(Camera *camera, Scene &scene, GameWidget *gameWidget) :
    QFrame(gameWidget),
    m_camera(camera),
    m_currentCameraComboBox(nullptr),
    m_theGameWidget(gameWidget),
    m_entitiesWithCamera(),
    m_lookupSpeed(0.1f),
    m_linearSpeed(1.f)
{
    Q_ASSERT (m_theGameWidget);

    connect(&scene, &Scene::cleared,
            [this] {
        m_entitiesWithCamera.clear();

        for (int i = 1; i < m_currentCameraComboBox->count(); i++) {
            m_currentCameraComboBox->removeItem(i);
        }
    });

    // Current camera combo box
    m_currentCameraComboBox = new QComboBox(this);
    m_currentCameraComboBox->addItem(FREE_CAMERA_NAME);

    connect(m_currentCameraComboBox, static_cast<void (QComboBox::*) (int)>(&QComboBox::currentIndexChanged),
            [this] (int currentIndex) {
        if (currentIndex == 0) {
            setCamera(nullptr);
        }
        else {
            Camera *camera = m_entitiesWithCamera[currentIndex - 1].component<Camera>().get();

            setCamera(camera);
        }
    });

    // Buttons
    const QString btnStyleSheet = "QToolButton"
                                  "{"
                                  "background-color: rgb(225,225,225); "
                                  "border: 1px solid black; "
                                  "border-radius: 2px"
                                  "}";
    const QSize btnSize {36, 36};

    //  Draw view button
    auto dragBtn = new DragToolButton(this);
    dragBtn->setStyleSheet(btnStyleSheet);
    dragBtn->setFixedSize(btnSize);
    dragBtn->setIcon(QIcon(":/editor/res/icons/pan_icon_32.png"));
    dragBtn->setToolTip("Drag view");

    connect(dragBtn, &DragToolButton::mouseDragged,
            [this] (float dx, float dy) {
        truckCamera(*m_camera, -dx, dy, m_linearSpeed, 1000.f);
    });

    //  Move forward/backward button
    auto moveForwardBtn = new DragToolButton(this);
    moveForwardBtn->setStyleSheet(btnStyleSheet);
    moveForwardBtn->setFixedSize(btnSize);
    moveForwardBtn->setIcon(QIcon(":/editor/res/icons/move_forward_icon_32.png"));
    moveForwardBtn->setToolTip("Move Forward/Backward");

    connect(moveForwardBtn, &DragToolButton::mouseDragged,
            [this] (float dx, float dy) {
        Q_UNUSED (dx);

        moveCameraForward(*m_camera, dy, m_linearSpeed, 1000.f);
    });

    // Assemble
    auto *myLayout = new QHBoxLayout(this);
    myLayout->setContentsMargins(0, 0, 0, 0);

    myLayout->addStretch();

    myLayout->addWidget(m_currentCameraComboBox);
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

        Q_EMIT cameraChanged(camera);
    }
}

void CameraControls::onEntityComponentAdded(entityx::Entity entity)
{
    Q_ASSERT (entity);

    if (!entity.has_component<Camera>()) {
        return;
    }

    const auto cameraHandled = std::find(m_entitiesWithCamera.begin(),
                                         m_entitiesWithCamera.end(),
                                         entity);

    if (cameraHandled != m_entitiesWithCamera.end()) {
        return;
    }

    m_entitiesWithCamera.emplace_back(entity);

    const QString entityName = QString::number(entity.id().index());

    m_currentCameraComboBox->addItem(entityName);
}

void CameraControls::onEntityComponentRemoved(entityx::Entity entity)
{
    Q_ASSERT (entity);  // entity is not already destroyed

    if (!entity.has_component<Camera>()) {
        return;
    }

    const auto cameraHandled = std::find(m_entitiesWithCamera.begin(),
                                         m_entitiesWithCamera.end(),
                                         entity);

    if (cameraHandled == m_entitiesWithCamera.end()) {
        return;
    }

    m_entitiesWithCamera.erase(cameraHandled);

    const QString entityName = QString::number(entity.id().index());

    m_currentCameraComboBox->removeItem(m_currentCameraComboBox->findText(entityName));
}

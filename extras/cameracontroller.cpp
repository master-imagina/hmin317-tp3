#include "cameracontroller.h"

#include <QKeyEvent>

#include "render/camera.h"

#include "cameraactions.h"


CameraController::CameraController(QObject *parent) :
    QObject(parent),
    m_keyDirection(KeyDirection::None),
    m_moveSpeed(100.f),
    m_linearSpeed(1.f),
    m_turboFactor(4.f),
    m_oldMousePos(),
    m_lastMouseDelta(),
    m_lastMousePosOnPress(),
    m_lastMousePos(),
    m_turboKeyPressed(false),
    m_isRotateAroundTargetPressed(false),
    m_isTruckBtnPressed(false)
{}

CameraController::~CameraController()
{}

bool CameraController::eventFilter(QObject *obj, QEvent *e)
{
    const QEvent::Type eventType = e->type();

    if (eventType == QEvent::KeyPress) {
        auto keyEvent = static_cast<QKeyEvent *>(e);

        keyPressEvent(keyEvent);

        return false;
    }
    else if (eventType == QEvent::KeyRelease) {
        auto keyEvent = static_cast<QKeyEvent *>(e);

        keyReleaseEvent(keyEvent);

        return false;
    }
    else if (eventType == QEvent::MouseButtonPress) {
        auto mouseEvent = static_cast<QMouseEvent *>(e);

        mousePressEvent(mouseEvent);

        return false;
    }
    else if (eventType == QEvent::MouseMove) {
        auto mouseEvent = static_cast<QMouseEvent *>(e);

        mouseMoveEvent(mouseEvent);

        return false;
    }
    else if (eventType == QEvent::MouseButtonRelease) {
        auto mouseEvent = static_cast<QMouseEvent *>(e);

        mouseReleaseEvent(mouseEvent);

        return false;
    }

    return QObject::eventFilter(obj, e);
}

QVector3D CameraController::computeDirectionFromKeys(Camera *camera) const
{
    const QVector3D viewVec = camera->viewVector().normalized();
    const QVector3D rightVec = camera->rightVector();

    QVector3D ret;

    switch (m_keyDirection) {
    case KeyDirection::None:
        break;
    case KeyDirection::Up:
        ret = viewVec;
        break;
    case KeyDirection::Down:
        ret = viewVec * -1;
        break;
    case KeyDirection::Left:
        ret = rightVec * -1;
        break;
    case KeyDirection::Right:
        ret = rightVec;
        break;
    }

    return ret;
}

void CameraController::updateCamera(Camera *camera, float dt)
{
    const QVector3D oldEye = camera->eyePos();
    const QVector3D oldTarget = camera->targetPos();

    const float speed = (m_turboKeyPressed) ? m_moveSpeed * m_turboFactor : m_moveSpeed;
    const float dtFactor = (dt / 1000.0f);
    const float moveAmount = dtFactor * speed;

    const QVector3D moveDirection = computeDirectionFromKeys(camera);

    const QVector3D newEye = moveDirection * moveAmount + oldEye;
    const QVector3D newTarget = moveDirection * moveAmount + oldTarget;

    camera->setEyePos(newEye);
    camera->setTargetPos(newTarget);

    if (m_isRotateAroundTargetPressed) {
        const float angle = 2.f * dtFactor * m_linearSpeed;

        rotateCameraAroundTarget(*camera, angle);
    }
    else if (m_isTruckBtnPressed) {
        truckCamera(*camera,
                    -m_lastMouseDelta.x() * dtFactor * m_linearSpeed,
                    m_lastMouseDelta.y() * dtFactor * m_linearSpeed);
    }
}

float CameraController::moveSpeed() const
{
    return m_moveSpeed;
}

void CameraController::setMoveSpeed(float moveSpeed)
{
    if (m_moveSpeed != moveSpeed) {
        m_moveSpeed = moveSpeed;
    }
}

float CameraController::linearSpeed() const
{
    return m_linearSpeed;
}

void CameraController::setLinearSpeed(float linearSpeed)
{
    if (m_linearSpeed != linearSpeed) {
        m_linearSpeed = linearSpeed;
    }
}

float CameraController::turboFactor() const
{
    return m_turboFactor;
}

void CameraController::setTurboFactor(float turboFactor)
{
    if (m_turboFactor != turboFactor) {
        m_turboFactor = turboFactor;
    }
}

void CameraController::keyPressEvent(QKeyEvent *e)
{
    const int key = e->key();

    KeyDirection newDirection = KeyDirection::None;

    if (key == Qt::Key_Z) {
        newDirection = KeyDirection::Up;
    }
    else if (key == Qt::Key_S) {
        newDirection = KeyDirection::Down;
    }
    else if (key == Qt::Key_Q) {
        newDirection = KeyDirection::Left;
    }
    else if (key == Qt::Key_D) {
        newDirection = KeyDirection::Right;
    }

    else if  (key == Qt::Key_R) {
        m_isRotateAroundTargetPressed = true;
    }

    m_keyDirection = (KeyDirection) ((int) m_keyDirection | (int) newDirection);

    if (e->modifiers() & Qt::ShiftModifier) {
        m_turboKeyPressed = true;
    }
}

void CameraController::keyReleaseEvent(QKeyEvent *e)
{
    const int key = e->key();

    KeyDirection directionToExit = KeyDirection::None;

    if (key == Qt::Key_Z) {
        directionToExit = KeyDirection::Up;
    }
    else if (key == Qt::Key_S) {
        directionToExit = KeyDirection::Down;
    }
    else if (key == Qt::Key_Q) {
        directionToExit = KeyDirection::Left;
    }
    else if (key == Qt::Key_D) {
        directionToExit = KeyDirection::Right;
    }

    else if  (key == Qt::Key_R) {
        m_isRotateAroundTargetPressed = false;
    }

    else {
        directionToExit = KeyDirection::None;
    }

    m_keyDirection = (KeyDirection) ((int) m_keyDirection & (~(int) directionToExit));

    if (key == Qt::Key_Shift) {
        m_turboKeyPressed = false;
    }
}

void CameraController::mousePressEvent(QMouseEvent *e)
{
    // /!\ Allow 'infinite' mouse move, don't remove.
    m_oldMousePos = e->pos();

    const Qt::MouseButton mouseBtn = e->button();

/*    if (mouseBtn == Qt::MouseButton::RightButton) {
        m_lookupBtnPressed = true;
    }
    else */if (mouseBtn == Qt::MouseButton::RightButton) {
        m_isTruckBtnPressed = true;
    }

    // /!\ Allow 'infinite' mouse move, don't remove.
    m_lastMousePosOnPress = m_lastMousePos = m_oldMousePos;
}

void CameraController::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);

/*    if (m_lookupBtnPressed) {
        m_lookupBtnPressed = false;
    }
    else */if (m_isTruckBtnPressed) {
        m_isTruckBtnPressed = false;
    }

    m_lastMouseDelta = QPoint();
}

void CameraController::mouseMoveEvent(QMouseEvent *e)
{
    // Compute mouse delta
    const QPoint newMousePos = e->pos();

    const QPoint mouseOffset = m_oldMousePos - newMousePos;
    const float dx = mouseOffset.x();
    const float dy = mouseOffset.y();

    m_lastMouseDelta.setX(dx);
    m_lastMouseDelta.setY(dy);

    // /!\ Allow 'infinite' mouse move, don't remove.
    m_oldMousePos = newMousePos;

    // /!\ Allow 'infinite' mouse move, don't remove.
    m_lastMousePos = m_lastMousePosOnPress;
}

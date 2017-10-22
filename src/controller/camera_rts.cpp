#include "camera_rts.h"
#include "../gameobject/gameobject.h"

#include <QtMath>

CameraRTSController::CameraRTSController(GameObject *parent) :
    Component(parent),
    wheelDelta(0),
    distance(10.0f),
    translationSpeed(0.05f),
    rotationSpeed(15.0f),
    zoomSpeed(0.1f),
    maxZoom(2.0f),
    minZoom(0.5f),
    rotation(0.0f)
{
    zoom = minZoom;
}

void CameraRTSController::update() {
    if (!timer.isValid()) {
        timer.start();
        gameObject().transform().position = center + QVector3D(0.0f, 1.0f, 1.0f) * distance * (zoom > 0.0f ? 1.0f / (1.0f + zoom) : 1.0f - zoom);
        return;
    }
    float elapsed = (float)timer.elapsed() / 1000.0;
    for (size_t i = 0, sz = mousePressed.size(); i < sz; i++) {
        if (mousePressed[i] == Qt::LeftButton)
            rotation -= (float)mouseMove.x() * rotationSpeed * elapsed;
    }
    if (wheelDelta != 0) {
        zoom += (float)wheelDelta * zoomSpeed * elapsed;
        if (zoom < minZoom)
            zoom = minZoom;
        if (zoom > maxZoom)
            zoom = maxZoom;
    }
    float rad = qDegreesToRadians(rotation);
    QVector3D forward = gameObject().transform().rotation * QVector3D(0.0f, 0.0f, 1.0f);
    QVector3D moving;
    forward.setY(0);
    for (size_t i = 0, sz = keyPressed.size(); i < sz; i++) {
        switch (keyPressed[i]) {
        case Qt::Key_Z:
            moving += forward;
            break;
        case Qt::Key_S:
            moving -= forward;
            break;
        case Qt::Key_Q:
            moving -= QVector3D(-forward.z(), 0.0f, forward.x());
            break;
        case Qt::Key_D:
            moving += QVector3D(-forward.z(), 0.0f, forward.x());
            break;
        }
    }
    moving.normalize();
    moving *= translationSpeed;
    center += moving;
    gameObject().transform().position = center + QVector3D(-qSin(rad), (zoom >= 0.0f ? 1.0f / (1.0f + zoom) : 2.0f - 1.0f / (1.0f - zoom)), -qCos(rad)) * distance * (zoom > 0.0f ? 1.0f / (1.0f + zoom) : 1.0f - zoom);
    gameObject().transform().rotation = QQuaternion::fromDirection(QVector3D(qSin(rad), -qDegreesToRadians(45.0f) * (zoom >= 0.0f ? 1.0f / (1.0f + zoom) : 2.0f - 1.0f / (1.0f - zoom)), qCos(rad)), QVector3D(0.0f, 1.0f, 0.0f));
    timer.restart();
    wheelDelta = 0;
    mouseMove = QPoint();
}

void CameraRTSController::keyPressEvent(QKeyEvent * event) {
    keyPressed.push_back(event->key());
}

void CameraRTSController::keyReleaseEvent(QKeyEvent * event) {
    for (size_t i = 0, sz = keyPressed.size(); i < sz; i++) {
        if (keyPressed[i] == event->key()) {
            keyPressed.erase(keyPressed.begin() + i);
            break;
        }
    }
}

void CameraRTSController::mousePressEvent(QMouseEvent * event) {
    mousePressed.push_back(event->button());
    lastPos = event->pos();
}

void CameraRTSController::mouseReleaseEvent(QMouseEvent * event) {
    for (size_t i = 0, sz = mousePressed.size(); i < sz; i++) {
        if (mousePressed[i] == event->button()) {
            mousePressed.erase(mousePressed.begin() + i);
            break;
        }
    }
}

void CameraRTSController::mouseMoveEvent(QMouseEvent * event) {
    mouseMove += (event->pos() - lastPos);
    lastPos = event->pos();
}

void CameraRTSController::wheelEvent(QWheelEvent * event) {
    wheelDelta += event->delta();
}

void CameraRTSController::destroy() {
    removeComponent();
    delete this;
}

int CameraRTSController::component() const {
    return COMPONENT_CONTROLLER_CAMERA_RTS_ID;
}

void CameraRTSController::clone(GameObject *c) {
    c->addComponent<CameraRTSController>();
}

bool CameraRTSController::isInstance(Component *c) {
    return (c->component() == COMPONENT_CONTROLLER_CAMERA_RTS_ID);
}

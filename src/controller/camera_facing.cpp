#include "camera_facing.h"
#include "../gameobject/gameobject.h"
#include "../gameobject/camera.h"

CameraFacingController::CameraFacingController(GameObject *parent) : Component(parent) {}

void CameraFacingController::update() {
    gameObject().transform().rotation = QQuaternion::fromDirection(gameObject().transform().position - Camera::mainCamera->gameObject().transform().position, QVector3D(0.0f, 1.0f, 0.0f));
}

void CameraFacingController::destroy() {
    removeComponent();
    delete this;
}

int CameraFacingController::component() const {
    return COMPONENT_CONTROLLER_CAMERA_FACING_ID;
}

void CameraFacingController::clone(GameObject *c) {
    c->addComponent<CameraFacingController>();
}

bool CameraFacingController::isInstance(Component *c) {
    return (c->component() == COMPONENT_CONTROLLER_CAMERA_FACING_ID);
}

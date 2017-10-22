#include "camera.h"
#include "gameobject.h"

Camera * Camera::mainCamera = 0;

Camera::Camera(GameObject *parent) :
    Component(parent),
    perspective(true),
    fov(45.0f),
    zNear(0.1f),
    zFar(30.0f),
    aspect(16.0f / 9.0f)
{
    if (mainCamera == 0)
        mainCamera = this;
}

void Camera::toggleView() {
    perspective = !perspective;
}

QMatrix4x4 Camera::getProjection() const {
    QMatrix4x4 matrix;
    matrix.setToIdentity();
    if (perspective)
        matrix.perspective(fov, aspect, zNear, zFar);
    else
        matrix.ortho(-2.0f * aspect, 2.0f * aspect, -2.0f, 2.0f, zNear, zFar);
    return matrix;
}

QMatrix4x4 Camera::getView() const {
    QMatrix4x4 matrix;
    Transform& t = gameObject().transform();
    matrix.setToIdentity();
    matrix.lookAt(t.position, t.position + t.rotation * QVector3D(0.0f, 0.0f, 1.0f), t.rotation * QVector3D(0.0f, 1.0f, 0.0f));
    return matrix;
}

void Camera::toggleView(bool p) {
    perspective = p;
}

void Camera::lookAt(QVector3D pos) {
    Transform& t = gameObject().transform();
    t.rotation = QQuaternion::fromDirection(pos - t.position, QVector3D(0.0f, 1.0f, 0.0f));
}

void Camera::apply(QOpenGLShaderProgram * program) {
    program->setUniformValue("m_projection", getProjection());
    program->setUniformValue("m_view", getView());
    program->setUniformValue("v_camerapos", gameObject().transform().position);
}

void Camera::destroy() {
    removeComponent();
    delete this;
}

int Camera::component() const {
    return COMPONENT_CAMERA_ID;
}

void Camera::clone(GameObject *c) {
    Camera * cam = c->addComponent<Camera>();
    cam->perspective = perspective;
    cam->fov = fov;
    cam->zNear = zNear;
    cam->zFar = zFar;
    cam->aspect = aspect;
}

bool Camera::isInstance(Component *c) {
    return (c->component() == COMPONENT_CAMERA_ID);
}

#include "cameraactions.h"

#include <QQuaternion>
#include <QtMath>

#include "render/camera.h"
#include "render/aabb.h"


namespace {

void rotateCamera(Camera &camera, const QQuaternion &quat)
{
    camera.setUpVector(quat * camera.upVector());

    const QVector3D cameraToCenter = quat * camera.viewVector();

    camera.setTargetPos(camera.eyePos() + cameraToCenter);
}

} // anon namespace


void rotateCameraAroundTarget(Camera &camera, float angle)
{
    const QVector3D axis = camera.upVector();
    const QQuaternion quat = QQuaternion::fromAxisAndAngle(axis, angle);

    camera.setUpVector(quat * axis);
    camera.setEyePos(camera.targetPos() - quat * camera.viewVector());
}

void centerCameraOnBBox(Camera &camera, const AABoundingBox &aabb)
{
    const QVector3D aabbCenter = aabb.center();
    const float distanceFromCenter = aabb.radius().length() * 2;

    const QVector3D newEye(aabbCenter + QVector3D(distanceFromCenter,
                                                  distanceFromCenter,
                                                  distanceFromCenter));

    camera.setEyePos(newEye);
    camera.setTargetPos({aabbCenter.x(), 0.f, aabbCenter.z()});
}

void moveCamera(Camera &camera, const QVector3D &direction, float speed, float dt)
{
    const float dtFactor = dt / 1000. * speed;
    const QVector3D moveAmount = direction * dtFactor;

    camera.setEyePos(camera.eyePos() + moveAmount);
    camera.setTargetPos(camera.targetPos() + moveAmount);
}

void moveCameraForward(Camera &camera, float offset, float speed, float dt)
{
    const float dtFactor = dt / 1000. * speed;
    const QVector3D moveAmount = camera.viewVector().normalized() * offset * dtFactor;

    camera.setEyePos(camera.eyePos() + moveAmount);
    camera.setTargetPos(camera.targetPos() + moveAmount);
}

void truckCamera(Camera &camera, float dx, float dy, float speed, float dt)
{
    const QVector3D amountX = camera.rightVector() * dx;
    const QVector3D amountY = camera.upVector() * dy;

    moveCamera(camera, amountX + amountY, speed, dt);
}

void panCamera(Camera &camera, float dx, float speed, float dt)
{
    const float dtFactor = dt / 1000. * speed;

    const float angle = dx * dtFactor;
    const auto rotQuat = QQuaternion::fromAxisAndAngle(camera.upVector(),
                                                       angle);

    rotateCamera(camera, rotQuat);
}

void tiltCamera(Camera &camera, float dy, float speed, float dt)
{
    const float dtFactor = dt / 1000. * speed;

    const float angle = dy * dtFactor;

    const QVector3D upVec = camera.upVector();

    const auto xBasis = QVector3D::crossProduct(upVec, camera.viewVector().normalized()).normalized();

    const auto rotQuat = QQuaternion::fromAxisAndAngle(xBasis,
                                                       -angle);

    rotateCamera(camera, rotQuat);
}

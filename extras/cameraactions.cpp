#include "cameraactions.h"

#include <QQuaternion>

#include "render/camera.h"
#include "render/aabb.h"


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

#include "cameraactions.h"

#include <QQuaternion>

#include "camera.h"
#include "aabb.h"


void rotateCameraAroundTarget(Camera *camera, float angle)
{
    const QVector3D axis = camera->upVector();
    const QQuaternion quat = QQuaternion::fromAxisAndAngle(axis, angle);

    camera->setUpVector(quat * axis);
    camera->setEyePos(camera->targetPos() - quat * camera->viewVector());
}

void centerCameraOnBBox(Camera *camera, const AABoundingBox &aabb)
{
    const QVector3D aabbCenter = aabb.center();
    const float distanceFromCenter = aabb.radius().length() * 2;

    const QVector3D newEye(aabbCenter + QVector3D(distanceFromCenter,
                                                  distanceFromCenter,
                                                  distanceFromCenter));

    camera->setEyePos(newEye);
    camera->setTargetPos({aabbCenter.x(), 0.f, aabbCenter.z()});
}

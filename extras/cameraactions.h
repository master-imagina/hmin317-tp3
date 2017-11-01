#ifndef CAMERAACTIONS_H
#define CAMERAACTIONS_H

class AABoundingBox;
class Camera;

class QVector3D;


void rotateCameraAroundTarget(Camera *camera, float angle);

void centerCameraOnBBox(Camera *camera, const AABoundingBox &aabb);

void moveCamera(Camera *camera, const QVector3D &amount);
void moveCameraForward(Camera *camera, float offset);
void truckCamera(Camera *camera, float dx, float dy);

#endif // CAMERAACTIONS_H

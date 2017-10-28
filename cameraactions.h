#ifndef CAMERAACTIONS_H
#define CAMERAACTIONS_H

class AABoundingBox;
class Camera;


void rotateCameraAroundTarget(Camera *camera, float angle);

void centerCameraOnBBox(Camera *camera, const AABoundingBox &aabb);

#endif // CAMERAACTIONS_H

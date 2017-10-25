#ifndef CAMERAFIRST_H
#define CAMERAFIRST_H

#include "external/glm/mat4x4.hpp"
#include "camera.h"

class FirstPersonCamera: public Camera
{
public:
    FirstPersonCamera();
    void move(float dx, float dy , int z, int s, int q, int d, int dHeigh);

    QMatrix4x4 getViewMatrix();

private:
    glm::vec3 lookAt;

    float distanceFromPivot;
    float angleAroundPivot;

    QMatrix4x4 viewMatrix;

};


#endif // CAMERAFIRST_H

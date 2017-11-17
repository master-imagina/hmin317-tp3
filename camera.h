#ifndef CAMERA_H
#define CAMERA_H

#include <QVector2D>
#include <QOpenGLWidget>
#include <QMatrix4x4>

typedef enum Camera_Movement {
    Z,
    S,
    Q,
    D,
    C,
    W
} Camera_Movement;

class Camera
{
public:
    Camera(QVector3D position = QVector3D(-5.0f, 25.0f, -5.0f), QVector3D up = QVector3D(0.0f, 1.0f, 0.0f), float yaw = 45.0f, float pitch = -30.0f)
        : cameraFront(0.0f, 0.0f, -1.0f)
    {
        cameraPos = position;
        worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        updateCameraVectors();
    }
    void processKeyPress(Camera_Movement movement);
    void processMouseMovement(float xoffset, float yoffset);
    void orbitAround(QMatrix4x4 &matrix, float y, float p);
    void lookAt(QMatrix4x4 &matrix);
    void setTarget();

    QVector3D getFront();
    QVector3D getWorldUp();
    QVector3D getRight();

private:
    float pitch, yaw;
    QVector3D cameraPos, cameraFront, cameraUp, cameraRight, worldUp, target;

    void updateCameraVectors();
};

#endif // CAMERA_H

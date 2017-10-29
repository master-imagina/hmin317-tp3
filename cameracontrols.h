#ifndef CAMERACONTROLS_H
#define CAMERACONTROLS_H

#include <QFrame>

class Camera;

class CameraControls : public QFrame
{
    Q_OBJECT

public:
    explicit CameraControls(Camera *camera, QWidget *parent = nullptr);

    Camera *getCamera() const;
    void setCamera(Camera *camera);

private:
    Camera *m_camera;

    float m_lookupSpeed;            // Speed when using the lookup button
    float m_linearSpeed;            // Speed when using all other buttons
};

#endif // CAMERACONTROLS_H

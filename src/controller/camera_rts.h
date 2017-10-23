#ifndef CAMERA_RTS_H
#define CAMERA_RTS_H

#include "../gameobject/component.h"

#include <vector>

#include <QElapsedTimer>
#include <QPoint>
#include <QVector3D>

class CameraRTSController : public Component {

    QElapsedTimer timer;

    std::vector<int> keyPressed;

    std::vector<int> mousePressed;

    QPoint lastPos;

    QPoint mouseMove;

    int wheelDelta;

public:

    QVector3D center;

    float distance;
    float translationSpeed;
    float rotationSpeed;
    float zoomSpeed;
    float maxZoom;
    float minZoom;

    float rotation;
    float zoom;

    // Basic constructor
    CameraRTSController(GameObject * parent);

    // Update callback
    virtual void update() override;

    // Key press event callback
    virtual void keyPressEvent(QKeyEvent * event) override;

    // Key release event callback
    virtual void keyReleaseEvent(QKeyEvent * event) override;

    // Mouse press event callback
    virtual void mousePressEvent(QMouseEvent * event) override;

    // Mouse release event callback
    virtual void mouseReleaseEvent(QMouseEvent * event) override;

    // Mouse move event callback
    virtual void mouseMoveEvent(QMouseEvent * event) override;

    // Mouse wheel event callback
    virtual void wheelEvent(QWheelEvent * event) override;

    // Component override
    virtual void destroy() override;

    // Component override
    virtual int component() const override;

    // Component override
    virtual void clone(GameObject * c) override;

    // Indicate if the componant is an instance of CameraRTSController
    static bool isInstance(Component * c);

};

#endif // CAMERA_RTS_H

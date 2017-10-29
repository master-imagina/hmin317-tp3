#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QObject>
#include <QVector3D>


class QKeyEvent;
class QMouseEvent;

class Camera;


class CameraController : public QObject
{
    Q_OBJECT

public:
    CameraController(QObject *parent = nullptr);
    ~CameraController();

    bool eventFilter(QObject *obj, QEvent *e) override;

    void updateCamera(Camera *camera, float dt);

    float moveSpeed() const;
    void setMoveSpeed(float moveSpeed);

    float linearSpeed() const;
    void setLinearSpeed(float linearSpeed);

    float turboFactor() const;
    void setTurboFactor(float turboFactor);

private:
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

    QVector3D computeDirectionFromKeys(Camera *camera) const;

private:
    enum KeyDirection
    {
        None = 0,
        Up = 1,
        Down = 2,
        Right = 4,
        Left = 8
    };

    KeyDirection m_keyDirection;

    float m_moveSpeed;
    float m_linearSpeed;
    float m_turboFactor;

    QPoint m_oldMousePos;
    QPoint m_lastMouseDelta;
    QPoint m_lastMousePosOnPress;
    QPoint m_lastMousePos;

    bool m_turboKeyPressed;
    bool m_isRotateAroundTargetPressed;
    bool m_isTruckBtnPressed;
};

#endif // CAMERACONTROLLER_H

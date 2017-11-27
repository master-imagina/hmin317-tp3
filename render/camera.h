#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>

class QDataStream;


class Camera
{
public:
    Camera();
    ~Camera() = default;

    QVector3D eyePos() const;
    void setEyePos(const QVector3D &eyePos);

    QVector3D targetPos() const;
    void setTargetPos(const QVector3D &targetPos);

    QVector3D upVector() const;
    void setUpVector(const QVector3D &upVector);

    QVector3D viewVector() const;
    QVector3D rightVector() const;

    float aspectRatio() const;
    void setAspectRatio(float aspectRatio);

    QMatrix4x4 viewMatrix();
    QMatrix4x4 projectionMatrix();
    QMatrix4x4 worldMatrix();

private:
    void setViewMatrixDirty();
    void setProjectionMatrixDirty();

private:
    QVector3D m_eyePos;
    QVector3D m_targetPos;
    QVector3D m_upVec;

    float m_aspectRatio;

    bool m_isViewMatrixDirty;
    bool m_isProjectionMatrixDirty;
    bool m_isWorldMatrixDirty;

    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_worldMatrix;
};


QDataStream &operator<<(QDataStream &os, const Camera &camera);
QDataStream &operator>>(QDataStream &os, Camera &camera);


#endif // CAMERA_H

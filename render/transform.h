#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QMatrix4x4>
#include <QVector3D>

class QDataStream;


class Transform
{
public:
    Transform();
    ~Transform() = default;

    QVector3D translate() const;
    void setTranslate(const QVector3D &vec);

    QVector3D rotation() const;
    void setRotation(const QVector3D &angles);

    QVector3D scale() const;
    void setScale(const QVector3D &factors);

    QMatrix4x4 matrix();

private:
    QMatrix4x4 m_matrix;

    QVector3D m_translateVec;
    QVector3D m_rotateAngles;
    QVector3D m_scaleFactors;

    bool m_isMatrixDirty;
};


QDataStream &operator<<(QDataStream &os, const Transform &transform);
QDataStream &operator>>(QDataStream &os, Transform &transform);

#endif // TRANSFORM_H

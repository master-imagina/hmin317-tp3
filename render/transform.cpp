#include "transform.h"

#include <QDataStream>
#include <QQuaternion>


Transform::Transform() :
    m_matrix(),
    m_translateVec(),
    m_rotateAngles(),
    m_scaleFactors(1., 1., 1.),
    m_isMatrixDirty(true)
{}

QVector3D Transform::translate() const
{
    return m_translateVec;
}

void Transform::setTranslate(const QVector3D &vec)
{
    if (m_translateVec != vec) {
        m_translateVec = vec;

        m_isMatrixDirty = true;
    }
}

QVector3D Transform::rotation() const
{
    return m_rotateAngles;
}

void Transform::setRotation(const QVector3D &angles)
{
    if (m_rotateAngles != angles) {
        m_rotateAngles = angles;

        m_isMatrixDirty = true;
    }
}

QVector3D Transform::scale() const
{
    return m_scaleFactors;
}

void Transform::setScale(const QVector3D &factors)
{
    if (m_scaleFactors != factors) {
        m_scaleFactors = factors;

        if (m_scaleFactors.x() == 0) {
            m_scaleFactors.setX(1.);
        }
        if (m_scaleFactors.y() == 0) {
            m_scaleFactors.setY(1.);
        }
        if (m_scaleFactors.z() == 0) {
            m_scaleFactors.setZ(1.);
        }

        m_isMatrixDirty = true;
    }
}

QMatrix4x4 Transform::matrix()
{
    if (m_isMatrixDirty) {
        m_matrix.setToIdentity();

        m_matrix.translate(m_translateVec);
        m_matrix.rotate(QQuaternion::fromEulerAngles(m_rotateAngles));
        m_matrix.scale(m_scaleFactors);

        m_isMatrixDirty = false;
    }

    return m_matrix;
}


QDataStream &operator<<(QDataStream &os, const Transform &transform)
{
    os << transform.translate()
       << transform.rotation()
       << transform.scale();
}

QDataStream &operator>>(QDataStream &os, Transform &transform)
{
    QVector3D position;
    QVector3D rotation;
    QVector3D scale;

    os >> position;
    os >> rotation;
    os >> scale;

    transform.setTranslate(position);
    transform.setRotation(rotation);
    transform.setScale(scale);
}

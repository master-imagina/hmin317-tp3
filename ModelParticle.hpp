#ifndef TP3_MODELPARTICLE_HPP
#define TP3_MODELPARTICLE_HPP

#include <QtGui/QImage>
#include <QtGui/QVector3D>
#include <QtGui/QOpenGLTexture>

class ModelParticle
{
public:
    explicit ModelParticle(QOpenGLTexture *texture, float life = 1.f, float mass = 1.f, float ground = 0.f,
                           const QVector3D &zigzag = QVector3D());
    virtual ~ModelParticle();

    void setTexture(QOpenGLTexture *texture);
    QOpenGLTexture* getTexture() const;

    void setMass(float mass);
    float getMass() const;

    void setGround(float ground);
    float getGround() const;

    void setLife(float life);
    float getLife() const;

    void setZigzag(const QVector3D &zigzag);
    const QVector3D &getZigzag() const;

    static const float MASS;

private:
    QOpenGLTexture *m_texture;
    float m_mass, m_ground, m_life; // Masse en gramme
    QVector3D m_zigzag;
};


#endif //TP3_MODELPARTICLE_HPP

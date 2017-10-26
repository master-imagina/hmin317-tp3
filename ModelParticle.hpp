#ifndef TP3_MODELPARTICLE_HPP
#define TP3_MODELPARTICLE_HPP

#include <QtGui/QImage>
#include <QtGui/QVector3D>
#include <QtGui/QOpenGLTexture>

class ModelParticle
{
public:
    explicit ModelParticle(QOpenGLTexture *texture, float life = 10.f, float mass = 0.05f, float ground = -0.f,
                           QVector3D size = QVector3D(0.5f, 0.5f, 0), bool dieOnGround = false);
    virtual ~ModelParticle();

    void setTexture(QOpenGLTexture *texture);
    QOpenGLTexture* getTexture() const;

    void setMass(float mass);
    float getMass() const;

    void setGround(float ground);
    float getGround() const;

    void setLife(float life);
    float getLife() const;

    void setSize(const QVector3D &size);
    const QVector3D &getSize() const;

    void setDieOnGround(bool dieOnGround);
    bool isDieOnGround() const;

    static const float MASS;

private:
    QOpenGLTexture *m_texture;
    float m_mass, m_ground, m_life; // Masse en gramme
    QVector3D m_size;
    bool m_dieOnGround;
};


#endif //TP3_MODELPARTICLE_HPP

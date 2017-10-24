#ifndef TP3_MODELPARTICLE_HPP
#define TP3_MODELPARTICLE_HPP

#include <QtGui/QImage>
#include <QtGui/QVector3D>

class ModelParticle
{
public:
    explicit ModelParticle(QImage *texture, float life = 1.f, float mass = 1.f, float ground = 0.f,
                           const QVector3D& zigzag = QVector3D());
    virtual ~ModelParticle();

    void setTexture(QImage *texture);
    QImage *getTexture() const;

    void setMass(float mass);
    float getMass() const;

    void setGround(float ground);
    float getGround() const;

    void setLife(float life);
    float getLife() const;

    void setZigzag(const QVector3D &zigzag);
    const QVector3D &getZigzag() const;


private:
    QImage *m_texture;
    float m_mass, m_ground, m_life; // Masse en gramme
    QVector3D m_zigzag;

    static const float MASS;
};


#endif //TP3_MODELPARTICLE_HPP

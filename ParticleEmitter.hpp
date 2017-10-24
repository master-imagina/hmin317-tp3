#ifndef TP3_PARTICLEEMITTER_HPP
#define TP3_PARTICLEEMITTER_HPP


#include <vector>
#include "Particle.hpp"

class ParticleEmitter
{
public:
    ParticleEmitter(unsigned int nbParticles, const ModelParticle &model, QVector3D position,
                        float height, float radius);
    ~ParticleEmitter();

    void setNbParticles(unsigned int nbParticles);
    unsigned int getNbParticles() const;

    void setHeight(float height);
    float getHeight() const;

    void setRadius(float radius);
    float getRadius() const;

    void setModel(const ModelParticle &model);
    const ModelParticle &getModel() const;

    void setPostion(const QVector3D &postion);
    const QVector3D &getPostion() const;

    void update(float delta);
    void draw(QOpenGLShaderProgram *program);

private:
    unsigned int m_nbParticles;
    float m_height, m_radius;
    ModelParticle m_model;
    QVector3D m_postion;
    std::vector<Particle> m_particles;

    void resetParticlePosition(Particle &particule);
};


#endif //TP3_PARTICLEEMITTER_HPP

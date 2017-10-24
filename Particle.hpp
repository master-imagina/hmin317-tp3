#ifndef TP3_PARTICLE_HPP
#define TP3_PARTICLE_HPP


#include <QtGui/QVector3D>
#include <QtGui/QOpenGLShaderProgram>
#include "ModelParticle.hpp"
#include "geometryengine.h"

class Particle
{
public:
    Particle(const ModelParticle& model, QVector3D position = QVector3D());
    Particle(const Particle& p);
    ~Particle();

    Particle& operator=(const Particle& p);

    bool isAlive() const;

    void setActualLife(float actualLife);
    float getActualLife() const;

    void setActualZigZag(const QVector3D &actualZigZag);
    const QVector3D &getActualZigZag() const;

    void setPosition(const QVector3D &pos);
    const QVector3D &getPosition() const;

    void setModel(const ModelParticle &model);
    const ModelParticle &getModel() const;

    void update(float delta);
    void draw(QOpenGLShaderProgram *program);

private:
    bool m_alive;
    float m_actualLife;
    QVector3D m_actualZigZag, m_pos;
    ModelParticle m_model;
    GeometryEngine m_plan;
};


#endif //TP3_PARTICLE_HPP

#ifndef TP3_PARTICLE_HPP
#define TP3_PARTICLE_HPP


#include <QtGui/QVector3D>
#include <QtGui/QOpenGLShaderProgram>
#include "ModelParticle.hpp"
#include "geometryengine.h"

class Particle
{
public:
    Particle(const ModelParticle& model, QVector3D position = QVector3D(), QVector3D size = QVector3D(1.f,1.f,0));
    ~Particle();

    bool isAlive() const;

    void setActualLife(float actualLife);
    float getActualLife() const;

    void setActualZigZag(const QVector3D &actualZigZag);
    const QVector3D &getActualZigZag() const;

    void setPosition(const QVector3D &pos);
    const QVector3D &getPosition() const;

    const QVector3D &getSize() const;

    void setSize(const QVector3D &size);

    void setModel(const ModelParticle &model);
    const ModelParticle &getModel() const;

    void update(float delta);
    void draw(QMatrix4x4 projection, QOpenGLShaderProgram *program);

private:
    bool m_alive;
    float m_actualLife;
    QVector3D m_actualZigZag, m_pos, m_size;
    ModelParticle m_model;
    GeometryEngine m_plan;
};


#endif //TP3_PARTICLE_HPP

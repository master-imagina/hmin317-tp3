#include "Particle.hpp"

Particle::Particle(const ModelParticle &model, QVector3D position, QVector3D size):
    m_alive(true), m_actualLife(0), m_pos(position), m_size(size), m_model(model)
{
    m_actualLife = m_model.getLife();
    m_plan.initPlaneGeometry();
}

Particle::~Particle()
{}

bool Particle::isAlive() const
{
    return m_alive;
}

void Particle::setActualLife(float actualLife)
{
    m_actualLife = actualLife;
}

float Particle::getActualLife() const
{
    return m_actualLife;
}

void Particle::setActualZigZag(const QVector3D &actualZigZag)
{
    m_actualZigZag = actualZigZag;
}

const QVector3D &Particle::getActualZigZag() const
{
    return m_actualZigZag;
}

void Particle::setPosition(const QVector3D &pos)
{
    m_pos = pos;
}

const QVector3D &Particle::getPosition() const
{
    return m_pos;
}

void Particle::setSize(const QVector3D &size)
{
    m_size = size;
}

const QVector3D &Particle::getSize() const
{
    return m_size;
}

void Particle::setModel(const ModelParticle &model)
{
    m_model = model;
}

const ModelParticle &Particle::getModel() const
{
    return m_model;
}

void Particle::update(float delta)
{
    m_actualLife = m_actualLife < delta ? 0 : m_actualLife - delta;
    m_pos.setY(m_pos.y() - (ModelParticle::MASS * m_model.getMass() * delta));
}

void Particle::draw(QMatrix4x4 projection, QOpenGLShaderProgram *program)
{
    m_model.getTexture()->bind();

    QMatrix4x4 matrix;
    matrix.translate(m_pos);
    matrix.scale(m_size);

    // Set modelview-projection matrix
    //program->setUniformValue("mvp_matrix", projection * matrix);

    // Use texture unit 0 which contains cube.png
    program->setUniformValue("texture", 0);

    // Draw cube geometry
    m_plan.drawPlaneGeometry(program);
}

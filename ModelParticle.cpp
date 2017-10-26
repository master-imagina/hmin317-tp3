#include "ModelParticle.hpp"

const float ModelParticle::MASS = 9.98f;

ModelParticle::ModelParticle(QOpenGLTexture *texture, float life, float mass, float ground, QVector3D size,
                             bool dieOnGround) :
    m_texture(texture), m_mass(mass), m_ground(ground), m_life(life), m_size(size), m_dieOnGround(dieOnGround)
{}

ModelParticle::~ModelParticle()
{}

QOpenGLTexture * ModelParticle::getTexture() const
{
    return m_texture;
}

void ModelParticle::setTexture(QOpenGLTexture *texture)
{
    m_texture = texture;
}

float ModelParticle::getMass() const
{
    return m_mass;
}

void ModelParticle::setMass(float mass)
{
    m_mass = mass;
}

float ModelParticle::getGround() const
{
    return m_ground;
}

void ModelParticle::setGround(float ground)
{
    m_ground = ground;
}

float ModelParticle::getLife() const
{
    return m_life;
}

void ModelParticle::setLife(float life)
{
    m_life = life;
}

void ModelParticle::setSize(const QVector3D &size)
{
    m_size = size;
}

const QVector3D &ModelParticle::getSize() const
{
    return m_size;
}

void ModelParticle::setDieOnGround(bool dieOnGround)
{
    m_dieOnGround = dieOnGround;
}

bool ModelParticle::isDieOnGround() const
{
    return m_dieOnGround;
}


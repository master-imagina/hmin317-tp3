#include <omp.h>
#include <iostream>
#include "ParticleEmitter.hpp"

using namespace std;

ParticleEmitter::ParticleEmitter(unsigned int nbParticles, const ModelParticle &model, QVector3D position,
                                 float height, float radius):
    m_nbParticles(nbParticles), m_height(height), m_radius(radius), m_model(model), m_postion(position)
{
    for(unsigned int i=0; i < m_nbParticles; ++i)
    {
        m_particles.emplace_back(m_model);
        resetParticlePosition(m_particles[m_particles.size() - 1]);
    }
    srand(time(NULL));
}

ParticleEmitter::~ParticleEmitter()
{
    m_particles.clear();
}

void ParticleEmitter::setNbParticles(unsigned int nbParticles)
{
    if(nbParticles > m_nbParticles)
    {
        for(unsigned int i = 0; i < (nbParticles - m_nbParticles); ++i)
        {
            m_particles.emplace_back(m_model);
            resetParticlePosition(*m_particles.end());
        }
    }
    else if(nbParticles < m_nbParticles)
        m_particles.erase(m_particles.begin(), m_particles.begin() + (m_nbParticles - nbParticles));

    m_nbParticles = nbParticles;
}

unsigned int ParticleEmitter::getNbParticles() const
{
    return m_nbParticles;
}

void ParticleEmitter::setHeight(float height)
{
    m_height = height;
}

float ParticleEmitter::getHeight() const
{
    return m_height;
}

void ParticleEmitter::setRadius(float radius)
{
    m_radius = radius;
}

float ParticleEmitter::getRadius() const
{
    return m_radius;
}

void ParticleEmitter::setModel(const ModelParticle &model)
{
    m_model = model;
    for(auto& particule: m_particles)
        particule.setModel(model);
}

const ModelParticle &ParticleEmitter::getModel() const
{
    return m_model;
}

void ParticleEmitter::setPostion(const QVector3D &postion)
{
    m_postion = postion;
}

const QVector3D &ParticleEmitter::getPostion() const
{
    return m_postion;
}

void ParticleEmitter::update(float delta)
{
    #pragma omp parallel for private(m_particles)
    for(auto& particle: m_particles)
    {
        if(particle.isAlive())
            particle.update(*this, delta);
        else
            resetParticlePosition(particle);
    }
}

void ParticleEmitter::draw(QMatrix4x4 &proj, QOpenGLShaderProgram *program)
{
    for(auto& particle: m_particles)
        particle.draw(proj, program);
}

void ParticleEmitter::resetParticlePosition(Particle &particule)
{
    float radius = static_cast<float>(rand() % 31415) / 10000.f;
    float random = static_cast<float>(rand() % 100) / 100.f;

    float posX = cosf(radius) * m_radius * random;
    float posZ = sinf(radius) * m_radius * random;

    particule.setPosition(QVector3D(posX + m_postion.x(), m_height + m_postion.y(), posZ + m_postion.z()));

    if(rand()%5 == 1) // Un change sur cinq d'être reveillé
        particule.resetLife();
}

void ParticleEmitter::resetAllParticles()
{
    for(auto& particle: m_particles)
        resetParticlePosition(particle);
}

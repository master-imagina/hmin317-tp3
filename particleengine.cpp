#include "particleengine.h"
#include <iostream>

ParticleEngine::ParticleEngine()
    : particlePosData()
{
    initializeOpenGLFunctions();
    arrayBuffer.create();

    initParticles();
}

ParticleEngine::~ParticleEngine()
{
    arrayBuffer.destroy();
}

void ParticleEngine::initParticles() {
    particleContainer[0] = Particle(QVector3D(0.0f, 5.0f, 0.0f)
                                    , QVector3D(1.0f, 1.0f, 1.0f)
                                    , 0.0f
                                    , 180.0f
                                    , 0.7f);
    particleContainer[1] = Particle(QVector3D(0.0f, 5.0f, 0.0f)
                                    , QVector3D(1.0f, 1.0f, 1.0f)
                                    , 0.0f
                                    , 165.0f
                                    , 0.7f);
    particlePosData.push_back(particleContainer[0].getPosition());
    particlePosData.push_back(particleContainer[1].getPosition());
    arrayBuffer.bind();
    arrayBuffer.allocate(particlePosData.data(), particlePosData.size() * sizeof(QVector3D));
}

void ParticleEngine::updateParticles() {
    particlePosData.clear();
    for(Particle &p : particleContainer) {
        p.update();
        particlePosData.push_back(p.getPosition());
    }

    arrayBuffer.bind();
    arrayBuffer.allocate(particlePosData.data(), particlePosData.size() * sizeof(QVector3D));
}

void ParticleEngine::drawParticles(QOpenGLShaderProgram *program) {
    arrayBuffer.bind();
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, 0);
    glDrawArrays(GL_POINTS, 0, particlePosData.size());
}


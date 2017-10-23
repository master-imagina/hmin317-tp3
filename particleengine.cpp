#include "particleengine.h"
#include <iostream>

ParticleEngine::ParticleEngine()
    : countParticules(0)
    , particlePosData()
{
    initializeOpenGLFunctions();
    //particlePosData = new QVector3D[MAX_PARTICULES];
    arrayBuffer.create();

    initParticles();
}

ParticleEngine::~ParticleEngine()
{
    arrayBuffer.destroy();
   // delete particlePosData;
}

void ParticleEngine::initParticles() {
    particleContainer[0] = Particle(QVector3D(0.0f, 5.0f, 0.0f)
                                    , QVector3D(1.0f, 1.0f, 1.0f)
                                    , 0.0f
                                    , 180.0f
                                    , 1.0f);
    particleContainer[1] = Particle(QVector3D(0.0f, 8.0f, 0.0f)
                                    , QVector3D(1.0f, 1.0f, 1.0f)
                                    , 0.0f
                                    , 180.0f
                                    , 1.0f);
    /*particlePosData[0] = particleContainer[0].getPosition();
    particlePosData[1] = particleContainer[1].getPosition();*/
    particlePosData.push_back(particleContainer[0].getPosition());
    particlePosData.push_back(particleContainer[1].getPosition());
    arrayBuffer.bind();
    arrayBuffer.allocate(particlePosData.data(), particlePosData.size() * sizeof(QVector3D));
    countParticules = 2;
}

void ParticleEngine::drawParticles(QOpenGLShaderProgram *program) {
    arrayBuffer.bind();
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeArray(vertexLocation, particlePosData.data(), 0);
    glDrawArrays(GL_POINTS, 0, particlePosData.size());
}


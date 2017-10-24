#include "particleengine.h"
#include <iostream>

ParticleEngine::ParticleEngine()
    : particlePosSizeData()
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
                                    , 0.7f
                                    , 5000
                                    , 10.0f);
    particlePosSizeData.push_back(particleContainer[0].getPosSize());
    particlePosSizeData.push_back(particleContainer[1].getPosSize());
    arrayBuffer.bind();
    arrayBuffer.allocate(particlePosSizeData.data(), particlePosSizeData.size() * sizeof(QVector4D));
}

void ParticleEngine::updateParticles() {
    particlePosSizeData.clear();
    for(Particle &p : particleContainer) {
        p.update();
        particlePosSizeData.push_back(p.getPosSize());
    }

    arrayBuffer.bind();
    arrayBuffer.allocate(particlePosSizeData.data(), particlePosSizeData.size() * sizeof(QVector4D));
}

void ParticleEngine::drawParticles(QOpenGLShaderProgram *program) {
    arrayBuffer.bind();
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 4, 0);
    glDrawArrays(GL_POINTS, 0, particlePosSizeData.size());
}


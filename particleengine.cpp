#include "particleengine.h"
#include <iostream>

ParticleEngine::ParticleEngine(ParticleType t)
    : particlesData()
    , lastUsedParticles(0)
    , lastTime(0)
    , type(t)
{
    initializeOpenGLFunctions();
    arrayBuffer.create();
    initParticles();
}

ParticleEngine::~ParticleEngine(){
    arrayBuffer.destroy();
}

void ParticleEngine::initParticles() {
    arrayBuffer.bind();
    arrayBuffer.allocate(particlesData.data(), particlesData.size() * sizeof(ParticleData));
}

void ParticleEngine::updateParticles() {
    particlesData.clear();
    for(Particle &p : particleContainer) {
        if(p.isAlive()) {
            p.update();
            particlesData.emplace_back(p.getPosSize(), p.getColor());
        }
    }
    arrayBuffer.bind();
    arrayBuffer.allocate(particlesData.data(), particlesData.size() * sizeof(ParticleData));
}

void ParticleEngine::drawParticles(QOpenGLShaderProgram *program) {
    arrayBuffer.bind();
    int vertexLocation = program->attributeLocation("a_position");
    int vertexColor = program->attributeLocation("a_color");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 4, sizeof(ParticleData));
    program->enableAttributeArray(vertexColor);
    program->setAttributeBuffer(vertexColor, GL_FLOAT, sizeof(QVector4D), 4, sizeof(ParticleData));
    glDrawArrays(GL_POINTS, 0, particlesData.size());
}

void ParticleEngine::generateParticles(float maxPerSeconde) {
    double delta = 0.01;
    if(lastTime == 0) {
        lastTime = QTime::currentTime().msecsSinceStartOfDay();
    } else {
        delta = QTime::currentTime().msecsSinceStartOfDay() - lastTime ;
        lastTime = QTime::currentTime().msecsSinceStartOfDay();
    }
    int newParticles = (int) (delta * maxPerSeconde);
    if(newParticles > (int) (0.016f * maxPerSeconde)) {
        newParticles = (int) (0.016f * maxPerSeconde);
    }
    for(int i = 0; i < newParticles; ++i) {
        int index = findUnusedParticles();
        particleContainer[index] = Particle::generateNewParticle(type);
        particlesData.emplace_back(particleContainer[index].getPosSize(), particleContainer[index].getColor());
    }
}

int ParticleEngine::findUnusedParticles() {
    for(int i = lastUsedParticles; i < MAX_PARTICULES; ++i) {
        if(!particleContainer[i].isAlive()) {
            lastUsedParticles = i;
            return i;
        }
    }
    for(int i = 0; i < lastUsedParticles; ++i) {
        if(!particleContainer[i].isAlive()) {
            lastUsedParticles = i;
            return i;
        }
    }
    return 0;
}

void ParticleEngine::setParticleType(ParticleType t) {
    type = t;
}

#include "particleengine.h"
#include <iostream>

ParticleEngine::ParticleEngine()
    : particlePosSizeData()
    , lastUsedParticles(0)
    , gen(rd())
    , lastTime(0)
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
    arrayBuffer.allocate(particlePosSizeData.data(), particlePosSizeData.size() * sizeof(QVector4D));
}

void ParticleEngine::updateParticles() {
    particlePosSizeData.clear();
    for(Particle &p : particleContainer) {
        if(p.isAlive()) {
            p.update();
            particlePosSizeData.push_back(p.getPosSize());
        }
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

void ParticleEngine::generateParticles() {
    double delta = 0.01;
    if(lastTime == 0) {
        lastTime = QTime::currentTime().msecsSinceStartOfDay();
    } else {
        delta = QTime::currentTime().msecsSinceStartOfDay() - lastTime ;
        lastTime = QTime::currentTime().msecsSinceStartOfDay();
    }
    float seuilGeneration = 200.0f;
    int newParticles = (int) (delta * seuilGeneration);
    if(newParticles > (int) (0.016f * seuilGeneration)) {
        newParticles = (int) (0.016f * seuilGeneration);
    }
    for(int i = 0; i < newParticles; ++i) {
        int index = findUnusedParticles();
        particleContainer[index] = ParticleEngine::generateSnowParticle();
        particlePosSizeData.push_back(particleContainer[index].getPosSize());
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

Particle ParticleEngine::generateSnowParticle() {
    std::uniform_real_distribution<> disXZ(0.0, 20.0);
    std::uniform_real_distribution<> disY(4.0, 5.0);
    std::uniform_real_distribution<> disSp(0.5, 1.0);
    std::uniform_int_distribution<> disPhi(160, 170);
    std::uniform_real_distribution<> disSi(1.0f, 2.0f);
    return Particle(QVector3D(disXZ(gen), disY(gen), disXZ(gen))
                    , QVector3D(1.0f, 1.0f, 1.0f)
                    , 0.0f
                    , disPhi(gen)
                    , disSp(gen)
                    , 5000
                    , disSi(gen));
}

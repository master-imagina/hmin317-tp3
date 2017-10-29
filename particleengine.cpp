#include "particleengine.h"
#include <iostream>

ParticleEngine::ParticleEngine()
    : particlesData()
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

Particle ParticleEngine::generateSnowParticle() {
    std::uniform_real_distribution<> disXZ(0.0, 20.0);
    std::uniform_real_distribution<> disY(4.0, 5.0);
    std::uniform_real_distribution<> disSp(0.5, 1.0);
    std::uniform_int_distribution<> disPhi(160, 170);
    std::uniform_real_distribution<> disSi(3.0f, 4.0f);
    return Particle(QVector3D(disXZ(gen), disY(gen), disXZ(gen))
                    , QVector4D(1.0f, 1.0f, 1.0f, 1.0f)
                    , 0.0f
                    , disPhi(gen)
                    , disSp(gen)
                    , 5000
                    , disSi(gen));
}

#include "particulesengine.h"

ParticuleEngine::ParticuleEngine(ParticleType t, int maxParticle)
    : vParticlesData()
    , lastUsedParticle(0)
    , lastTime(0)
    , type(t)
    , maxParticle(maxParticle)
{
    initializeOpenGLFunctions();
    arrayBuffer.create();
    initParticles();

}

ParticuleEngine::~ParticuleEngine()
{
    arrayBuffer.destroy();
}

void ParticuleEngine::initParticles()
{
    arrayBuffer.bind();
    arrayBuffer.allocate(vParticlesData.data(), vParticlesData.size() * sizeof(ParticleData));
}

void ParticuleEngine::uptadeParticles()
{
    vParticlesData.clear();
    for(Particle &p : particleContainer){
        if(p.isAlive()){
            p.update();
            vParticlesData.emplace_back(p.getPositionAndSize(), p.getColor());
        }
    }
    arrayBuffer.bind();
    arrayBuffer.allocate(vParticlesData.data(), vParticlesData.size() * sizeof(ParticleData));
}

void ParticuleEngine::drawParticles(QOpenGLShaderProgram *prog)
{
    arrayBuffer.bind();
    int vertexLocation = prog->attributeLocation("a_position");
    int vertexColor = prog->attributeLocation("a_color");

    prog->enableAttributeArray(vertexLocation);
    prog->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 4, sizeof(ParticleData));

    prog->enableAttributeArray(vertexColor);
    prog->setAttributeBuffer(vertexColor, GL_FLOAT, sizeof(QVector4D), 4, sizeof(ParticleData));

    glDrawArrays(GL_POINTS, 0, vParticlesData.size());
}

void ParticuleEngine::generateParticles(float seuil)
{
    double delta = 0.01;
    if(lastTime == 0) {
        lastTime = QTime::currentTime().msecsSinceStartOfDay();
    } else {
        delta = QTime::currentTime().msecsSinceStartOfDay() - lastTime;
        lastTime = QTime::currentTime().msecsSinceStartOfDay();
    }

    int nbNewParticles = (int) delta * seuil;
    if (nbNewParticles > (int) (0.016f * seuil)){
        nbNewParticles = (int) (0.016f * seuil);
    }
    for (int i = 0; i < nbNewParticles; ++i) {
        int index = findUnusedParticle();
        particleContainer[index] = Particle::generateSeasonParticle(type);
        vParticlesData.emplace_back(particleContainer[index].getPositionAndSize(),
                                    particleContainer[index].getColor());
    }
}

int ParticuleEngine::findUnusedParticle()
{
    for (int i = 0; i < maxParticle; ++i) {
        if (!particleContainer[i].isAlive()) {
            lastUsedParticle = i;
            return i;
        }
    }
    for(int i = 0; i < lastUsedParticle; ++i) {
        if(!particleContainer[i].isAlive()) {
            lastUsedParticle = i;
            return i;
        }
    }
    return 0;
}

void ParticuleEngine::setParticleType(ParticleType t)
{
    type = t;
}

void ParticuleEngine::setMaxParticle(int nb)
{
    maxParticle = nb;
}

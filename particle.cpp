#include "particle.h"
#include "geometryengine.h"

Particle::Particle()
    : pos(QVector3D(0.0f, 0.0f, 0.0f))
    , color(QVector4D(1.0f, 1.0f, 1.0f, 1.0f))
    , life(1000.0f)
    , velocity(QVector3D(0.0f, 0.0f, 0.0f))
    , lastTime(0)
    , size(5.0f)
{
    originalLife = life;
    originalSize = size;
}

Particle::Particle(QVector3D pos, QVector4D color, float theta, float phi, float speed, int life, float size)
    : pos(pos)
    , color(color)
    , life(life)
    , size(size)
    , lastTime(0)
{
    float thetaRad = qDegreesToRadians(theta);
    float phiRad = qDegreesToRadians(phi);
    float x = speed * sin(phiRad) * cos(thetaRad);
    float z = speed * sin(phiRad) * sin(thetaRad);
    float y = speed * cos(phiRad);

    velocity = QVector3D(x, y, z);
    originalSize = this->size;
    originalLife = this->life;
}

void Particle::update()
{
    double delta = 0;
    if(lastTime == 0){
        lastTime = QTime::currentTime().msecsSinceStartOfDay();
    } else {
        delta = QTime::currentTime().msecsSinceStartOfDay() - lastTime;
        lastTime = QTime::currentTime().msecsSinceStartOfDay();
    }

    life -= delta;

    if(life > 0){
        pos += (velocity * (delta / 100.0f));

        if(pos.y() <= 0.0f) {
            life = 0;
        }

        float ageRatio = life /originalLife;

        if(size > 2.0f){
            size = originalSize * ageRatio;
        }
    }
}

bool Particle::isAlive()
{
    return life > 0.0f;
}

QVector3D Particle::getPosition()
{
    return pos;
}

QVector4D Particle::getPositionAndSize()
{
    return QVector4D(pos.x(), pos.y(), pos.z(), size);
}

QVector4D Particle::getColor()
{
    return color;
}

Particle Particle::generateSeasonParticle(ParticleType type)
{
    switch(type){
    case ParticleType::Snow:
        return generateSnowParticle();
    case ParticleType::Rain:
        return generateRainParticle();
    default:
        break;
    }
    return generateSnowParticle();
}

Particle Particle::generateSnowParticle()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    //distribution sur la largeur et longueur
    std::uniform_real_distribution<> disXZ(0.0, (float) MAP_SIZE);
    //distribution sur la hauteur
    std::uniform_real_distribution<> disY(8.0, 13.0);
    //variation de la vitesse
    std::uniform_real_distribution<> disSp(2.0, 2.5);
    //variation de l'angle
    std::uniform_int_distribution<> disPhi(160, 170);
    //variation de la taille
    std::uniform_real_distribution<> disSi(3.0f, 4.0f);
    return Particle(QVector3D(disXZ(gen), disY(gen), disXZ(gen))
                    , QVector4D(1.0f, 1.0f, 1.0f, 1.0f)
                    , 0.0f
                    , disPhi(gen)
                    , disSp(gen)
                    , 6000
                    , disSi(gen));
}

Particle Particle::generateRainParticle()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    //distribution sur la largeur et longueur
    std::uniform_real_distribution<> disXZ(0.0, (float) MAP_SIZE);
    //distribution sur la hauteur
    std::uniform_real_distribution<> disY(8.0, 13.0);
    //variation de la vitesse
    std::uniform_real_distribution<> disSp(8.0, 9.0);
    //variation de l'angle
    std::uniform_int_distribution<> disPhi(170, 175);
    //variation de la taille
    std::uniform_real_distribution<> disSi(1.3f, 1.3f);
    return Particle(QVector3D(disXZ(gen), disY(gen), disXZ(gen))
                    , QVector4D(0.1f, 0.7f, 0.9f, 1.0f)
                    , 0.0f
                    , disPhi(gen)
                    , disSp(gen)
                    , 3000
                    , disSi(gen));
}

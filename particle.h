#ifndef PARTICLE_H
#define PARTICLE_H


class Particle
{
public:
    Particle(float,float,float, float,float,float, float, float, float,float,float);
    bool Update(float deltatime);
    float GetX() {return x;}
    float GetY() {return y;}
    float GetZ() {return z;}
    float GetXm() {return xmove;}
    float GetYm() {return ymove;}
    float GetZm() {return zmove;}

private:
    float x;
    float y;
    float z;
    float xmove;
    float ymove;
    float zmove;
    float gravitypull;
    float size;
    float r;
    float g;
    float b;
    float ttl;
};

#endif // PARTICLE_H

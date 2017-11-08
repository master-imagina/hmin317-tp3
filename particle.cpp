#include "particle.h"
#include <cmath>
#include <algorithm>
Particle::Particle(float _x,float _y,float _z, float _xm,float _ym,float _zm,
                   float _gp, float _sz, float _r,float _g,float _b)
{
    x = _x;
    y = _y;
    z = _z;
    xmove = _xm;
    ymove = _ym;
    zmove = _zm;
    gravitypull = _gp;
    size = _sz;
    r = _r;
    g = _g;
    b = _b;
    ttl = 6000.0;
}

bool Particle::Update(float DT)
{
    x += xmove*DT;
    y += ymove*DT;
    z += zmove*DT;
    z-= gravitypull;
    gravitypull*=1.05f;
    gravitypull = std::min (0.1f,gravitypull);
    ttl -= DT;

    if (z <0) return true;

    if (ttl <= 0) return true;
    else return false;
}

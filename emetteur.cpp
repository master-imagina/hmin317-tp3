#include "emetteur.h"

Emetteur::Emetteur(int nb_particles) : nb_particles(nb_particles), active(true)
{
    for (int i = 0 ; i < nb_particles ; i++) {
        particles.insert(i, new Particle(i));
    }
}

Emetteur::~Emetteur() {
    for (int i = 0 ; i < particles.length() ; i++) {
        delete particles[i];
        particles.removeAt(i);
    }
}

void Emetteur::drawParticles(QOpenGLShaderProgram *program) {
    if(!active) return;

    for (int i = 0 ; i < particles.length() ; i++) {
        particles[i]->drawParticle(program);
    }
}

#ifndef PARTICLES_H
#define PARTICLES_H

#include <QOpenGLFunctions_4_2_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#define MAX_PARTICLES 1000

struct Particle {
    QVector3D pos, speed;
    unsigned char r,g,b,a;
    float size, angle, weight;
    float life;
};

class Particles : public QOpenGLFunctions_4_2_Core
{
public:
    Particles();
    ~Particles();

    void generateParticles();
    void simulateParticles();
    void drawParticles(QOpenGLShaderProgram *program);

private:
    void initParticlesBuffer();
    void updateParticlesBuffer();
    int findUnusedParticles();

    QOpenGLBuffer billboard_vertex_buffer, particles_position_buffer, particles_color_buffer;
    QVector4D *particles_pos_size_data, *particles_color_data;
    Particle particlesContainer[MAX_PARTICLES];
    int particles_count, last_used_particle;
    double lastTime;
};

#endif // PARTICLES_H

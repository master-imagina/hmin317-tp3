#ifndef PARTICLES_H
#define PARTICLES_H

#include <QOpenGLFunctions_4_2_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#define MAX_PARTICLES 200

class Particles : public QOpenGLFunctions_4_2_Core
{
public:
    Particles();
    ~Particles();

    void drawParticles(QOpenGLShaderProgram *program);

private:
    void initParticlesBuffer();
    void updateParticlesBuffer();

    QOpenGLBuffer billboard_vertex_buffer, particles_position_buffer, particles_color_buffer;
    QVector4D *particles_pos_size_data, *particles_color_data;

    int particles_count;
};

#endif // PARTICLES_H

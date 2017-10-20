#include "particles.h"
#include <QTime>
#include <iostream>

Particles::Particles()
    : billboard_vertex_buffer(QOpenGLBuffer::VertexBuffer)
    , particles_position_buffer(QOpenGLBuffer::VertexBuffer)
    , particles_color_buffer(QOpenGLBuffer::VertexBuffer)
    , lastTime(0)
    , last_used_particle(0)
{
    initializeOpenGLFunctions();
    // Generate VBOs
    billboard_vertex_buffer.create();
    billboard_vertex_buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    particles_position_buffer.create();
    particles_position_buffer.setUsagePattern(QOpenGLBuffer::StreamDraw);
    particles_color_buffer.create();
    particles_color_buffer.setUsagePattern(QOpenGLBuffer::StreamDraw);
    for(int i = 0; i < MAX_PARTICLES; ++i) {
        particlesContainer[i].life = -1.0f;
    }
    particles_pos_size_data = new QVector4D[MAX_PARTICLES];
    particles_color_data =  new QVector4D[MAX_PARTICLES];
    initParticlesBuffer();
}

Particles::~Particles()
{
    billboard_vertex_buffer.destroy();
    particles_position_buffer.destroy();
    particles_color_buffer.destroy();
    delete particles_pos_size_data;
    delete particles_color_data;
}

void Particles::initParticlesBuffer() {
    static const GLfloat g_vertex_buffer_data[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
    };
    billboard_vertex_buffer.bind();
    billboard_vertex_buffer.allocate(g_vertex_buffer_data, sizeof(g_vertex_buffer_data));
    particles_position_buffer.bind();
    particles_position_buffer.allocate(MAX_PARTICLES * sizeof(QVector4D));
    particles_color_buffer.bind();
    particles_color_buffer.allocate(MAX_PARTICLES * sizeof(QVector4D));
}

void Particles::updateParticlesBuffer() {
    //update pos and size
    particles_position_buffer.bind();
    particles_position_buffer.allocate(MAX_PARTICLES * sizeof(QVector4D));
    particles_position_buffer.write(0, particles_pos_size_data, particles_count * sizeof(QVector4D));
    //update color
    particles_color_buffer.bind();
    particles_color_buffer.allocate(MAX_PARTICLES * sizeof(QVector4D));
    particles_color_buffer.write(0, particles_color_data, particles_count * sizeof(QVector4D));
}

void Particles::drawParticles(QOpenGLShaderProgram *program) {
    // Tell OpenGL which VBOs to use
    billboard_vertex_buffer.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation0 = program->attributeLocation("squareVertices");
    program->enableAttributeArray(vertexLocation0);
    program->setAttributeBuffer(vertexLocation0, GL_FLOAT, offset, 3);

    particles_position_buffer.bind();
    // Offset for position
    offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation1 = program->attributeLocation("syzs");
    program->enableAttributeArray(vertexLocation1);
    program->setAttributeBuffer(vertexLocation1, GL_FLOAT, offset, 4);

    particles_color_buffer.bind();
    // Offset for position
    offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation2 = program->attributeLocation("color");
    program->enableAttributeArray(vertexLocation2);
    program->setAttributeBuffer(vertexLocation2, GL_FLOAT, offset, 4);
    glVertexAttribDivisor(vertexLocation0, 0);
    glVertexAttribDivisor(vertexLocation1, 1);
    glVertexAttribDivisor(vertexLocation2, 1);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particles_count);
}

void Particles::generateParticles() {
    double delta = 0;
    if(lastTime == 0) {
        lastTime = QTime::currentTime().msecsSinceStartOfDay();
    } else {
        delta = (QTime::currentTime().msecsSinceStartOfDay() - lastTime) / 1000.0f ;
    }
    int newparticles = (int) (delta * 200.0);
    if (newparticles > (int)(0.016f*200.0)) // limit numbers of particles generated in a frame
        newparticles = (int)(0.016f*200.0);
    for(int i = 0; i < newparticles; ++i) {
        int index = findUnusedParticles();
        Particle &p = particlesContainer[index];
        p.life = 5.0f;
        p.pos = QVector3D(0.0f, 5.0f, 0.0f);
        p.speed = QVector3D(0.0f, -0.1f, 0.0f);
    }
}

void Particles::simulateParticles() {
    double delta = (QTime::currentTime().msecsSinceStartOfDay() - lastTime) / 1000.0f;
    lastTime = QTime::currentTime().msecsSinceStartOfDay();
    particles_count = 0;
    for(int i = 0; i < MAX_PARTICLES; ++i) {
        Particle &p = particlesContainer[i];
        if(p.life > 0.0f) {
            p.life -= delta;
            if(p.life > 0.0f) {
                p.speed += QVector3D(0.0f, -9.81f, 0.0f) * (float) delta * 0.1f;
                p.pos += p.speed * (float) delta;
                particles_pos_size_data[particles_count] = QVector4D(p.pos.x(), p.pos.y(), p.pos.z(), 10.0f);
                particles_color_data[particles_count] = QVector4D(1.0f, 1.0f, 1.0f, 1.0f);
                //std::cout << particles_pos_size_data[particles_count].x() << " " << particles_pos_size_data[particles_count].y() << " " << particles_pos_size_data[particles_count].z()<< std::endl;
            }
            ++particles_count;
        }
    }
    updateParticlesBuffer();
}

int Particles::findUnusedParticles() {
    for(int i=last_used_particle; i<MAX_PARTICLES; i++){
        if (particlesContainer[i].life < 0.0f){
            last_used_particle = i;
            return i;
        }
    }

    for(int i=0; i<last_used_particle; i++){
        if (particlesContainer[i].life < 0.0f){
            last_used_particle = i;
            return i;
        }
    }
    return 0; // All particles are taken, override the first one
}

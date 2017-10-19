#include "particles.h"

Particles::Particles()
    :billboard_vertex_buffer(QOpenGLBuffer::VertexBuffer)
    , particles_position_buffer(QOpenGLBuffer::VertexBuffer)
    , particles_color_buffer(QOpenGLBuffer::VertexBuffer)
{
    initializeOpenGLFunctions();
    // Generate VBOs
    billboard_vertex_buffer.create();
    billboard_vertex_buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    particles_position_buffer.create();
    particles_position_buffer.setUsagePattern(QOpenGLBuffer::StreamDraw);
    particles_color_buffer.create();
    particles_color_buffer.setUsagePattern(QOpenGLBuffer::StreamDraw);
    initParticlesBuffer();
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

}

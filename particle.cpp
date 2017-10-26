#include "particle.h"

Particle::Particle(int id) : id(id), lifeTime(10), active(true)
{
    initializeOpenGLFunctions();

    arrayBuf.create();
    indexBuf.create();
}

Particle::~Particle(){
    arrayBuf.destroy();
    indexBuf.destroy();
}

void Particle::initParticles() {
    particleData = new ParticleData[4];

    QVector3D *positon = new QVector3D(rand() % 512,rand() % 512, 5);

    for (int i = 0 ; i < 2 ; i++) {
        for (int j = 0 ; j < 2 ; j++) {
            particleData->position = *positon + QVector3D(i, j, 0);
            particleData->texCoord = QVector2D(i,j);
        }
    }

    GLushort indices[4];

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 3;

    arrayBuf.bind();
    arrayBuf.allocate(positon, 4 * sizeof(ParticleData));

    indexBuf.bind();
    indexBuf.allocate(indices, 4 * sizeof(GLushort));
}

void Particle:: drawParticle(QOpenGLShaderProgram *program) {
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(ParticleData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(ParticleData));

    // Draw plane geometry using indices from VBO 1
    glDrawElements(GL_QUADS, 1, GL_UNSIGNED_SHORT, 0);
}

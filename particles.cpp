#include "particles.h"

#include <iostream>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

Particles * Particles::instance = NULL;

Particles::Particles() {
    instance = this;

    texture = new QOpenGLTexture(QImage("flocon.png").mirrored());
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);

    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        return;

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        return;

    // Link shader pipeline
    if (!program.link())
        return;

    VertexData * vertices = new VertexData[max_particules * 4];
    for (unsigned int i = 0, sz = max_particules * 4; i < sz; i += 4) {
        vertices[i] = { QVector3D(-0.1f, -0.1f,  0.1f), QVector2D(0.0f, 0.0f) };
        vertices[i + 1] = { QVector3D(0.1f, -0.1f,  0.1f), QVector2D(1.0f, 0.0f) };
        vertices[i + 2] = { QVector3D(-0.1f, 0.1f,  0.1f), QVector2D(0.0f, 1.0f) };
        vertices[i + 3] = { QVector3D(0.1f, 0.1f,  0.1f), QVector2D(1.0f, 1.0f) };
    }

    // Indices for drawing cube faces using triangle strips.
    // Triangle strips can be connected by duplicating indices
    // between the strips. If connecting strips have opposite
    // vertex order then last index of the first strip and first
    // index of the second strip needs to be duplicated. If
    // connecting strips have same vertex order then only last
    // index of the first strip needs to be duplicated.
    GLushort * indices = new GLushort[max_particules * 6];
    for (unsigned int i = 0, j = 0, sz = max_particules * 4; i < sz; i += 4, j += 6) {
        indices[j] = i;
        indices[j] = i + 1;
        indices[j] = i + 2;
        indices[j] = i + 1;
        indices[j] = i + 2;
        indices[j] = i + 3;
    }

    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, max_particules * 4 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, max_particules * 6 * sizeof(GLushort));
}
Particles::~Particles() {
    delete texture;
}

Particles& Particles::getInstance() {
    return *instance;
}

void Particles::update() {}
void Particles::paint() {
    std::cout << "test0\n";
    texture->bind();
    std::cout << "test1\n";

    // Bind shader pipeline for use
    if (!program.bind())
        return;

    arrayBuf.bind();
    indexBuf.bind();
    std::cout << "test2\n";

    // Offset for position
    quintptr offset = 0;
    
    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program.attributeLocation("a_position");
    program.enableAttributeArray(vertexLocation);
    program.setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));
    std::cout << "test3\n";

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program.attributeLocation("a_texcoord");
    program.enableAttributeArray(texcoordLocation);
    program.setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));
    std::cout << "test4\n";

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, max_particules * 6 , GL_UNSIGNED_SHORT, 0);
    std::cout << "test5\n";
}
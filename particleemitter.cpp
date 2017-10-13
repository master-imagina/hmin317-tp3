#include "particleemitter.h"

#include <iostream>
ParticleEmitter::ParticleEmitter()
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();
    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();
}

ParticleEmitter::~ParticleEmitter()
{
    // Delete 2 VBOs
    arrayBuf.destroy();
    indexBuf.destroy();
}


void ParticleEmitter::Update(float DT)
{
    int k=0;
    for(QVector<Particle*>::iterator i=Particles.begin();i<Particles.end();i++)
    {
        Particle * p = Particles.at(k);
        bool f = p->Update(DT);
        if (f)
        {
            Particles.erase(i);
            i--;
            k--;
        }
        k++;
    }
    currentTime += DT;
    Particle * p = new Particle(0.0,0.0,5.0, -(250.0f/8000.0f)+(qrand()%500)/8000.0f,-(250.0f/8000.0f)+(qrand()%500)/8000.0f,0.02f ,0.1f ,1.0 ,1.0,1.0,1.0);
    std::cout << p->GetXm() << ":" << p->GetYm() << std::endl;
    if (currentTime > 0.1)
    {
        currentTime -=0.1;

        Particles.push_back(p);
    }

    std::cout <<"Particles: " <<  Particles.size() << std::endl;

}

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};


void ParticleEmitter::Render(QOpenGLShaderProgram *program)
{


    if (Particles.size()>0)
    {
        VertexData * vertices = (VertexData *)malloc(Particles.size()*6*sizeof(VertexData));
        GLuint * indices = (GLuint *)malloc(Particles.size()*6*sizeof(GLuint));

        int k=0;
        for(QVector<Particle*>::iterator i=Particles.begin();i<Particles.end();i++)
        {
            Particle * p = Particles.at(k);
            vertices[k*6+0] = {QVector3D(p->GetX(), p->GetY(), p->GetZ() ),QVector2D(0.0f, 0.0f)};
            indices[k*6+0] = (GLuint)(k*6+0);
            vertices[k*6+1] = {QVector3D(p->GetX()+1.0f, p->GetY(), p->GetZ() ),QVector2D(1.0f, 0.0f)};
            indices[k*6+1] = (GLuint)(k*6+1);
            vertices[k*6+2] = {QVector3D(p->GetX()+1.0f, p->GetY()+1.0f, p->GetZ() ),QVector2D(1.0f, 1.0f)};
            indices[k*6+2] = (GLuint)(k*6+2);

            vertices[k*6+3] = {QVector3D(p->GetX(), p->GetY(), p->GetZ() ),QVector2D(0.0f, 0.0f)};
            indices[k*6+3] = (GLuint)(k*6+3);
            vertices[k*6+4] = {QVector3D(p->GetX()+1.0f, p->GetY()+1.0f, p->GetZ() ),QVector2D(1.0f, 1.0f)};
            indices[k*6+4] = (GLuint)(k*6+4);
            vertices[k*6+5] = {QVector3D(p->GetX(), p->GetY()+1.0f, p->GetZ() ),QVector2D(0.0f, 1.0f)};
            indices[k*6+5] = (GLuint)(k*6+5);

            k++;
        }


        // Transfer vertex data to VBO 0
        arrayBuf.bind();
        arrayBuf.allocate(vertices, Particles.size()*6 * sizeof(VertexData));

        // Transfer index data to VBO 1
        indexBuf.bind();
        indexBuf.allocate(indices, Particles.size()* 6 * sizeof(GLuint));



        // Tell OpenGL which VBOs to use
        //arrayBuf.bind();
        //indexBuf.bind();

        // Offset for position
        quintptr offset = 0;

        // Tell OpenGL programmable pipeline how to locate vertex position data
        int vertexLocation = program->attributeLocation("a_position");
        program->enableAttributeArray(vertexLocation);
        program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

        // Offset for texture coordinate
        offset += sizeof(QVector3D);

        // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
        int texcoordLocation = program->attributeLocation("a_texcoord");
        program->enableAttributeArray(texcoordLocation);
        program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

        // Draw cube geometry using indices from VBO 1
        std::cout << "Rendering..." << std::endl;
        glDrawElements(GL_TRIANGLES, Particles.size()*6, GL_UNSIGNED_INT, 0);

        delete vertices;
        delete indices;

    }

}

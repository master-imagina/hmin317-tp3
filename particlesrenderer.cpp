#include "particlesrenderer.h"

ParticlesRenderer::ParticlesRenderer()
:indexBuf(QOpenGLBuffer::IndexBuffer)
{

}

void ParticlesRenderer::initParticuleRenderer()
{
    initializeOpenGLFunctions();

    snowFlakeTexture = new QOpenGLTexture(QImage(":/snowFlake.png").mirrored());

    snowFlakeTexture->setMinificationFilter(QOpenGLTexture::Linear);

    snowFlakeTexture->setMagnificationFilter(QOpenGLTexture::Linear);

    rainTexture = new QOpenGLTexture(QImage(":/rain.png").mirrored());

    rainTexture->setMinificationFilter(QOpenGLTexture::Linear);

    rainTexture->setMagnificationFilter(QOpenGLTexture::Linear);

    particlesArrayBuf.create();
    indexBuf.create();
    generateParticlesBuffer();

    particleRenderProgram = new QOpenGLShaderProgram;

    particleRenderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/particleRenderVertex.glsl");
    //particleRenderProgram->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/particleRenderGeo.glsl");
    particleRenderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/particleRenderFragment.glsl");


    // Link shader pipeline
    particleRenderProgram->link();
    glEnable(GL_PROGRAM_POINT_SIZE_EXT);
    glPointSize(15);
    glEnable(GL_POINT_SPRITE);
}

void ParticlesRenderer::generateParticlesBuffer()
{
    std::vector<QVector2D> index;
    std::vector<GLushort> indices;
    int k=0;
    for(int i=0;i<PARTICLE_MAX;i++){
        for(int j=0;j<PARTICLE_MAX;j++){

            index.push_back(QVector2D(i/(float)PARTICLE_MAX,j/(float)PARTICLE_MAX));
            indices.push_back(k++);
        }
    }

    particlesArrayBuf.bind();
    particlesArrayBuf.allocate(&index[0], index.size() * sizeof(QVector2D));
    m_nomberIndices = indices.size();
    indexBuf.bind();
    indexBuf.allocate(&indices[0], indices.size() * sizeof(GLushort));
}

void ParticlesRenderer::renderParticles(QOpenGLTexture *particlesTex, QMatrix4x4 mvp, float snowFactor,int calendar)
{

    particleRenderProgram->bind();
    particlesArrayBuf.bind();
    indexBuf.bind();
    particlesTex->bind(0);
    snowFlakeTexture->bind(1);
    rainTexture->bind(2);
    // Tell OpenGL programmable pipeline how to locate vertex position data
    int indexLocation = particleRenderProgram->attributeLocation("index");
    particleRenderProgram->enableAttributeArray(indexLocation);
    particleRenderProgram->setAttributeBuffer(indexLocation, GL_FLOAT, 0, 2, sizeof(QVector2D));
    particleRenderProgram->setUniformValue("particlesTex",0);
    particleRenderProgram->setUniformValue("snowFlake",1);
    particleRenderProgram->setUniformValue("rain",2);
    particleRenderProgram->setUniformValue("mvp",mvp);
    particleRenderProgram->setUniformValue("calendar",calendar);

    glDrawElements(GL_POINTS, m_nomberIndices, GL_UNSIGNED_SHORT, 0);
}



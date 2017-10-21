#ifndef PARTICULESSYSTEM_H
#define PARTICULESSYSTEM_H

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
//#include <QOpenGLExtraFunctions>

#define PARTICLE_MAX 256

class ParticulesSystem: protected QOpenGLFunctions
{
public:
    ParticulesSystem();

    void initParticuleSystem();
    void cleanUp();

    void proccessTextureParticles(QOpenGLTexture *heightMap, float snowFactor, int calendar);
    void generateQuad();
    void renderQuad(QOpenGLTexture *heightMap, float snowFactor, int calendar);

    QOpenGLTexture* getParticlesTexture();
    QOpenGLTexture* getExtraDataTexture();

private:
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;

    QOpenGLShaderProgram* particleProgram;

    QOpenGLTexture* particuleTexture;
    QOpenGLTexture* extraDataTexture;
    QOpenGLFramebufferObject  * captureFBO;
    int m_nomberIndices;
    QOpenGLFramebufferObjectFormat format;

};


#endif // PARTICULESSYSTEM_H

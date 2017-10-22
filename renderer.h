#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

#include "renderer/buffer.h"
#include "renderer/openglversion.h"

class Geometry;
class GeometryDesc;
class ParticleEffect;


class Renderer
{
public:
    Renderer();
    ~Renderer();

    void initialize();

    bool isDirty() const;
    void setDirty();
    void unsetDirty();

    void updateBuffers(Geometry *geom, ParticleEffect *particleEffect);
    void updateUniforms(const QVariantMap &uniforms);

    void render();

    void cleanup();

private:
    void sendVariantUniform(QOpenGLShaderProgram &program,
                            const QString &name, const QVariant &value);

private:
    void cglPrintAnyError();

    void setupVaoForBufferAndShader(GLuint vao,
                                    const GeometryDesc &geometryDesc,
                                    Buffer &vertexBuffer,
                                    const QOpenGLShaderProgram &program,
                                    Buffer *indexBuffer = nullptr);

private:
    OpenGLFuncs *gl;
    std::array<GLuint, 2> m_vaos;

    QOpenGLShaderProgram m_terrainShaderProgram;
    QOpenGLShaderProgram m_particlesShaderProgram;

    Buffer m_terrainArrayVbo;
    Buffer m_terrainIndexVbo;

    Buffer m_particlesVbo;

    bool m_isDirty;

    std::vector<QOpenGLShaderProgram *> m_shaderPrograms;
};

#endif // RENDERER_H

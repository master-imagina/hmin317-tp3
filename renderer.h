#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

class Geometry;
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

private:
    QOpenGLFunctions_3_3_Core *gl;
    std::array<GLuint, 2> m_vaos;

    QOpenGLShaderProgram m_terrainShaderProgram;

    QOpenGLShaderProgram m_particlesShaderProgram;
    GLuint m_particlesVbo;

    GLuint m_vertexVbo;
    GLuint m_indexVbo;

    bool m_isDirty;

    std::vector<QOpenGLShaderProgram *> m_shaderPrograms;
};

#endif // RENDERER_H

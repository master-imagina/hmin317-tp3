#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

class Geometry;


class Renderer : protected QOpenGLFunctions_3_3_Core
{
public:
    Renderer();
    ~Renderer();

    void initialize();

    void updateBuffers(Geometry *geom);
    void updateUniforms(const QVariantMap &uniforms);

    void render();

    void cleanup();

private:
    void sendVariantUniform(QOpenGLShaderProgram &program,
                            const QString &name, const QVariant &value);

private:
    void cglPrintAnyError();

private:
    QOpenGLVertexArrayObject m_vao;
    QOpenGLShaderProgram m_shaderProgram;

    GLuint m_vertexVbo;
    GLuint m_indexVbo;
};

#endif // RENDERER_H

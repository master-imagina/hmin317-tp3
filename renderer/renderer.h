#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

#include "buffermanager.h"
#include "glbuffer.h"
#include "glwrapper.h"
#include "openglversion.h"

class Geometry;
class Scene;

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void initialize();

    void updateUniforms(const QVariantMap &uniforms);

    std::vector<DrawCommand> prepareDrawCommands(Scene *scene);
    void render(Scene *scene, float dt);

    void cleanup();

private:
    void updateBuffers(const std::vector<Geometry *> &geoms);

private:
    void cglPrintAnyError();

private:
    BufferManager m_bufferManager;

    OpenGLFuncs *gl;
    GLWrapper m_glWrapper;

    std::array<QOpenGLShaderProgram, 2> m_shaderPrograms;
    std::array<GLuint, 2> m_vaos;
    std::array<GLBuffer, 2> m_arrayVbos;
    std::array<GLBuffer, 2> m_indexVbos;
};

#endif // RENDERER_H

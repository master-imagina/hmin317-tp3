#ifndef RENDERER_H
#define RENDERER_H

#include "buffermanager.h"
#include "glbuffer.h"
#include "glwrapper.h"
#include "openglversion.h"
#include "shadermanager.h"

class Geometry;
class Material;
class RenderPass;
class Scene;


class Renderer
{
public:
    Renderer();
    ~Renderer();

    void initialize(Scene *scene);
    void render(Scene *scene, float dt);

    void cleanup();

private:
    void updateDirtyBuffers(const std::vector<Geometry *> &geoms);
    void updatePassParameters(const std::vector<Material *> &materials);

    std::vector<DrawCommand> prepareDrawCommands(Scene *scene);

private:
    BufferManager m_bufferManager;
    ShaderManager m_shaderManager;

    OpenGLFuncs *m_gl;
    GLWrapper m_glWrapper;

    std::array<uint32, 2> m_shaderIds;
    std::array<GLuint, 2> m_vaos;
    std::array<GLBuffer, 2> m_arrayVbos;
    std::array<GLBuffer, 2> m_indexVbos;
};

#endif // RENDERER_H

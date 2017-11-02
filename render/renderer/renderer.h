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

    std::vector<uint32> m_vaos;
};

#endif // RENDERER_H

#ifndef RENDERER_H
#define RENDERER_H

#include "buffermanager.h"
#include "glbuffer.h"
#include "glwrapper.h"
#include "openglversion.h"
#include "shadermanager.h"
#include "vaomanager.h"

class Geometry;
class Material;
class RenderPass;

namespace entityx {
class Entity;
} // namespace Entity


class Renderer
{
public:
    Renderer();
    ~Renderer();

    void initialize();
    void startNewFrame();
    void prepareDrawCommand(entityx::Entity entity);
    void render(float dt);

    void cleanup();

private:
    DrawCommand createDrawCommand(Geometry &geometry, Material &material) const;

    void createGLResources(Geometry &geom, Material &material, DrawCommand &drawCmd);
    void updateDirtyBuffers(DrawCommand &drawCmd);
    void updatePassParameters(const DrawCommand &drawCmd);

private:
    BufferManager m_bufferManager;
    ShaderManager m_shaderManager;
    VaoManager m_vaoManager;

    OpenGLFuncs *m_gl;
    GLWrapper m_glWrapper;

    std::vector<DrawCommand> m_drawCommands;
};

#endif // RENDERER_H

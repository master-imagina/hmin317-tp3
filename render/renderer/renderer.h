#ifndef RENDERER_H
#define RENDERER_H

#include "buffermanager.h"
#include "glbuffer.h"
#include "glwrapper.h"
#include "openglversion.h"
#include "shadermanager.h"
#include "texturemanager.h"
#include "vaomanager.h"

class Camera;
class Geometry;
class Material;
class RenderPass;
class ShaderParam;
class Transform;

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
    void render(Camera &camera, float dt);

    void cleanup();

private:
    DrawCommand createDrawCommand(Geometry &geometry,
                                  Material &material,
                                  Transform &transform) const;

    void createGLResources(Geometry &geom,
                           Material &material,
                           DrawCommand &drawCmd);

    void updateDirtyBuffers(DrawCommand &drawCmd);
    void updatePassParameters(Camera &camera, const DrawCommand &drawCmd);

private:
    BufferManager m_bufferManager;
    ShaderManager m_shaderManager;
    VaoManager m_vaoManager;
    TextureManager m_textureManager;

    OpenGLFuncs *m_gl;
    GLWrapper m_glWrapper;

    std::vector<DrawCommand> m_drawCommands;
    std::vector<ShaderParam *> m_currentPassParams;
    std::vector<ShaderParam *> m_activeTextures;
};

#endif // RENDERER_H

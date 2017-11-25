#ifndef RENDERER_H
#define RENDERER_H

#include "managers/buffermanager.h"
#include "managers/shadermanager.h"
#include "managers/texturemanager.h"
#include "managers/vaomanager.h"

#include "glbuffer.h"
#include "glwrapper.h"
#include "openglversion.h"

#include "render/light.h"


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
    void prepareDrawCommand(Geometry &geom, Material &material, Transform &transform);
    void render(Camera &camera, float dt);

    void cleanup();

    //TODO Ugly, get rid of it
    void addLight(const Light &light);

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
    std::vector<Light> m_currentLights;
};

#endif // RENDERER_H

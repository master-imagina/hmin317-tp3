#include "quick.h"

#include <QColor>
#include <QVector3D>

#include "core/scene.h"

#include "render/geometry/geometry.h"

#include "render/material/material.h"
#include "render/material/renderpass.h"
#include "render/material/shaderparam.h"
#include "render/material/shaderprogram.h"
#include "render/material/shaderutils.h"
#include "render/material/texture.h"

#include "particleeffect.h"



entityx::Entity createParticleEffect(Scene &scene,
                                     const QVector3D &worldPos,
                                     const QVector3D &direction,
                                     int count,
                                     int maxLife,
                                     float radius,
                                     float speed,
                                     float size)
{
    // /!\ Assume the system engine has the following component dependency
    // ParticleEffect -> Geometry, Material
    entityx::Entity ret = scene.createEntity();

    entityx::ComponentHandle<ParticleEffect> effect = ret.assign<ParticleEffect>();
    effect->setWorldPos(worldPos);
    effect->setDirection(direction);
    effect->setCount(count);
    effect->setMaxLife(maxLife);
    effect->setRadius(radius);
    effect->setSpeed(speed);
    effect->setParticleSize(size);

    entityx::ComponentHandle<Geometry> geom = ret.component<Geometry>();
    VertexAttrib standardVertexAttrib {"vertexPos", 3, VertexAttrib::Type::Float, false, 0};
    geom->vertexLayout.addAttribute(standardVertexAttrib);

    uptr<ShaderProgram> shaderProgram = shaderProgramFromFile(
        "shaders/particle.vert",
        "shaders/particle.geom",
        "shaders/particle.frag"
    );

    entityx::ComponentHandle<Material> material = ret.component<Material>();
    RenderPass *basePass = material->addRenderPass("base");
    basePass->setShaderProgram(std::move(shaderProgram));

    material->setParam("particleColor", QColor(Qt::red));
    material->setParam("particleSize", size);
    material->setParam("particleTexture", QVariant::fromValue(Texture2D()));

    return ret;
}

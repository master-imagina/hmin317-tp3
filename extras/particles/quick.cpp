#include "quick.h"

#include <QColor>

#include "core/scene.h"

#include "render/geometry/geometry.h"

#include "render/material/material.h"
#include "render/material/renderpass.h"
#include "render/material/shaderparam.h"
#include "render/material/shaderprogram.h"
#include "render/material/shaderutils.h"
#include "render/material/texture.h"

#include "render/transform.h"

#include "particleeffect.h"


void createParticleEffect(entityx::Entity entity,
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
    entityx::ComponentHandle<ParticleEffect> effect = entity.assign<ParticleEffect>();

    entityx::ComponentHandle<Transform> transform = entity.component<Transform>();
    transform->setTranslate(worldPos);

    effect->setDirection(direction);
    effect->setCount(count);
    effect->setMaxLife(maxLife);
    effect->setRadius(radius);
    effect->setSpeed(speed);
    effect->setParticleSize(size);

    entityx::ComponentHandle<Geometry> geom = entity.component<Geometry>();
    geom->vertexLayout.addAttribute(defaultPositionAttrib());

    uptr<ShaderProgram> shaderProgram = shaderProgramFromFile(
                "shaders/particle.vert",
                "shaders/particle.geom",
                "shaders/particle.frag"
                );

    entityx::ComponentHandle<Material> material = entity.component<Material>();
    RenderPass *basePass = material->addRenderPass("base");
    basePass->setShaderProgram(std::move(shaderProgram));

    material->setParam("particleColor", QColor(Qt::red));
    material->setParam("particleSize", size);
    material->setParam("particleTexture", QVariant::fromValue(Texture2D()));
    material->setParam("textureFlag", 0.f);
}

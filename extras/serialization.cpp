#include "serialization.h"

#include "core/scene.h"

#include "extras/particles/particleeffect.h"
#include "extras/particles/quick.h"

#include "render/mesh.h"
#include "render/transform.h"


QDataStream &operator<<(QDataStream &os, const Scene &scene)
{
    os << (quint64) scene.count();

    for (entityx::Entity &entity : scene.entityCache()) {
        os << QString("entity");

        if (entity.has_component<Transform>()) {
            os << QString("transform");
            os << *entity.component<Transform>().get();
        }
        if (entity.has_component<Mesh>()) {
            os << QString("mesh");
            os << *entity.component<Mesh>().get();
        }
        if (entity.has_component<ParticleEffect>()) {
            os << QString("particleeffect");
            os << *entity.component<ParticleEffect>().get();
        }
        //TODO handle materials

        os << QString("endentity");
    }
}

QDataStream &operator>>(QDataStream &os, Scene &scene)
{
    quint64 entityCount = 0;
    os >> entityCount;

    for (int i = 0; i < entityCount; i++) {
        QString dummyEntityFlag;
        os >> dummyEntityFlag;

        entityx::Entity entity = scene.createEntity();

        QString endEntityFlag;
        os >> endEntityFlag;

        while (endEntityFlag != "endentity") {
            const QString componentType = endEntityFlag;

            if (componentType == "transform") {
                Transform transform;
                os >> transform;

                entity.replace<Transform>(transform);
            }
            else if (componentType == "mesh") {
                Mesh mesh;
                os >> mesh;

                entity.assign<Mesh>(mesh);
            }
            else if (componentType == "particleeffect") {
                ParticleEffect particleEffect;
                os >> particleEffect;

                createParticleEffect(entity,
                                     particleEffect.direction(),
                                     particleEffect.count(),
                                     particleEffect.maxLife(),
                                     particleEffect.radius(),
                                     particleEffect.speed(),
                                     particleEffect.particleSize(),
                                     particleEffect.spawnRate());
            }

            os >> endEntityFlag;
        }
    }
}
